////////////////////////////////
// 
// Main.cpp
// 作成開始日：2025/12/07
// 
// 作成日：2025/12/07
// 作業内容：#1
// 　　　追加：Begin(画面クリア)とEnd(表示)を繰り返して描画するようにした。
// 
// 作成日：2025/12/14
// 作業内容：#2
// 　　　追加：三角形描画処理の追加
// 
// 作成日：2025/12/21
// 作業内容：#3
// 　　　追加：ECS導入によりリニューアル
// 
// 作成日：2025/12/28
// 作業内容：#4
// 　　　追加：カメラ実装
// 
// 作成日：2025/12/28
// 作業内容：#5
// 　　　追加：カメラのキー入力移動
// 
// 作成日：2025/12/29
// 作業内容：#6
// 　　　追加：タイマーの実装
// 
// 作成日：2025/12/29
// 作業内容：#7
// 　　　追加：Inputシステムの追加
// 
// 作成日：2025/12/31
// 作業内容：#8
//       追加：オブジェクト表示
// 
// 
////////////////////////////////




#include <Windows.h>
#include "Core/Window.h"
#include "Core/Main.h"
#include "Graphics/DX11Device.h" // 描画するためのもの

// #3:ECS関連のヘッダー
#include "ECS//Coordinator.h"
#include "Components/Components.h"
#include "Systems/RenderSystem.h"

// #5:カメラキー入力移動に必要なヘッダー
#include "systems/CameraControlSystem.h"

// #6:ゲームタイマーに必要なヘッダー
#include "Core/GameTimer.h"

// #7:インプットクラスに必要なヘッダー
#include "Core/Input.h"

// #8:オブジェクト表示に必要なヘッダー
#include "Core/OBJLoader.h"
#include "Systems/RenderSystem.h"

// #9 オブジェクトの回転に必要なヘッダー
#include "Systems/Rotationsystem.h"

// #3:グローバル変数としてCoordinatorを用意（どこからでもアクセスできるようにするため）
Coordinator gCoordinator;
Input gInput; // どこからでも使える入力管理者



int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window;
	DX11Device dx11; // #1

	// 初期化 (ウィンドウ作成)
	if (!window.Init(hInstance, SCREEEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE))
	{
		return -1;
	}

	// #1：DirectX初期化（ウィンドウのハンドルを渡す）
	if (!dx11.Init(window.GetHandle(), SCREEEN_WIDTH, SCREEN_HEIGHT))
	{
		MessageBox(NULL, L"DirectX Init Failed", L"Error", MB_OK);
		return -1;
	}

	gCoordinator.Init();
	gInput.Init(); // #7:入力システムの初期化

	// #6:タイマーの初期化と作成
	GameTimer timer;
	timer.Reset();

	// #3:コンポーネントの登録
	// 「このゲームでは　TransformとMeshというデータを使います」と教える
	gCoordinator.RegisterComponent<Transform>();
	gCoordinator.RegisterComponent<Mesh>();
	gCoordinator.RegisterComponent<Camera>(); // Cameraコンポーネントの登録

	// #3:システム（RenderSystem）の登録
	auto renderSystem = gCoordinator.RegisterSystem<RenderSystem>();

	// #5:カメラ操作システムの登録
	auto cameraControlSystem = gCoordinator.RegisterSystem<CameraControlSystem>();

	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Transform>());
		signature.set(gCoordinator.GetComponentType<Camera>());
		gCoordinator.SetSystemSignature<CameraControlSystem>(signature);
	}

	// #8:回転システムの登録
	auto rotationSystem = gCoordinator.RegisterSystem<RotationSystem>();

	// #3:システムが担当する条件（シグネチャ）を設定
	// 「RenderSystemは、TransformとMeshの両方を持っているEntityだけを扱うよ」という設定
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Transform>());
		signature.set(gCoordinator.GetComponentType<Mesh>());
		gCoordinator.SetSystemSignature<RenderSystem>(signature);
	}

	// #3:システムの初期化（シェーダー読み込み等）
	renderSystem->Init(&dx11);
	///////////////////////////////////////////////
	// 
	// #3:ゲームオブジェクト（Entity）の作成テスト
	// 
	///////////////////////////////////////////////
	
	std::vector<Vertex> meshVertices; // 手打ちデータを消し、OBJ読み込み用のvectorを用意

	if (!OBJLoader::Load("C:\\cube.obj", meshVertices))
	{
		MessageBox(NULL, L"cube.obj Load Failed", L"Error", MB_OK);
		return - 1;
	}

	if (meshVertices.empty())
	{
		MessageBox(NULL, L"Mesh Vertices is Empty! (0)", L"Error", MB_OK);
		return -1;
	}

	for (int i = 0; i < 5; ++i) //ループを使って5個のEntityを作る
	{
		// 1. Entity（ID）を発行
		Entity box = gCoordinator.CreateEntity();

		// 2. 位置（Transform）を設定
		Transform trans;
		// X座標をずらして横一列に並べる ( -4.0, -2.0, 0.0, 2.0, 4.0 )
		trans.Position = XMFLOAT3((float)(i - 2) * 2.5f, 0.0f, 0.0f);
		trans.Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
		trans.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		gCoordinator.AddComponent(box, trans);

		// 3. 形（Mesh）を設定
		Mesh mesh;
		mesh.Vertices = meshVertices; // 読み込んだデータをコピーして渡す
		gCoordinator.AddComponent(box, mesh);
	}


	// #3:Entityを1つ作成
	Entity myEntity = gCoordinator.CreateEntity();

	// #3:位置情報（Transform）を追加
	gCoordinator.AddComponent(myEntity, Transform());

	// #3:形状情報（Mesh）を追加
	Mesh mesh;
	mesh.Vertices = meshVertices;
	gCoordinator.AddComponent(myEntity, mesh);

	////////////////////////////
	// 
	// #4:カメラエンティティの作成
	// 
	////////////////////////////

	Entity cameraEntity = gCoordinator.CreateEntity();

	// 位置設定:Z = -5.0f (三角形より後ろに下がる)
	Transform cameraTransform;
	cameraTransform.Position = XMFLOAT3(0.0f, 0.0f, -2.0f);
	cameraTransform.Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	gCoordinator.AddComponent(cameraEntity, cameraTransform);

	Camera cameraComp;
	// #6:ここで計算
	cameraComp.AspectRatio = (float)SCREEEN_WIDTH / (float)SCREEN_HEIGHT;
	gCoordinator.AddComponent(cameraEntity, cameraComp);

	




	// メインループ
	while (window.ProcessMessage())
	{

		// 時間計算
		timer.Tick();
		float dt = timer.DeltaTime();

		// #7:入力情報の更新
		gInput.Update();

		// カメラ更新(ｄｔを渡す)
		cameraControlSystem->Update(&gCoordinator, dt);

		rotationSystem->Update(&gCoordinator, dt);
	
		// #5:カメラ操作の更新（描画の前にやる）
		//cameraControlSystem->Update(&gCoordinator, cameraEntity);

		// #1：描画開始（画面を濃い青色でクリア）
		dx11.Begin(0.1f, 0.2f, 0.1f); // ウィンドウの色設定

		// #3:ECSによる描画
		// システムに「仕事して！」と指示するだけ
		renderSystem->Render(&gCoordinator, cameraEntity);

		// ここに将来以下の処理が入ります
		// Game.Update();
		// Game.Render();

		// #1：描画終了（画面を更新）
		dx11.End();
	}
	renderSystem->Shutdown();
	return 0;
}