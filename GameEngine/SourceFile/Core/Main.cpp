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


// #3:グローバル変数としてCoordinatorを用意（どこからでもアクセスできるようにするため）
Coordinator gCoordinator;



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

	// #3:コンポーネントの登録
	// 「このゲームでは　TransformとMeshというデータを使います」と教える
	gCoordinator.RegisterComponent<Transform>();
	gCoordinator.RegisterComponent<Mesh>();
	gCoordinator.RegisterComponent<Camera>(); // Cameraコンポーネントの登録

	// #3:システム（RenderSystem）の登録
	auto renderSystem = gCoordinator.RegisterSystem<RenderSystem>();

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
	
	// #3:三角形の頂点データ
	std::vector<Vertex> triangleVertices = {
		{ 0.0f, 0.2f, 0.0f }, //上
		{ 0.2f, -0.2f, 0.0f }, // 右下
		{ -0.2f, -0.2f, 0.0f } // 左下
	};

	// #3:Entityを1つ作成
	Entity myEntity = gCoordinator.CreateEntity();

	// #3:位置情報（Transform）を追加
	gCoordinator.AddComponent(myEntity, Transform());

	// #3:形状情報（Mesh）を追加
	Mesh mesh;
	mesh.Vertices = triangleVertices;
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

	// カメラ設定：今回はデフォルト設定のまま
	gCoordinator.AddComponent(cameraEntity, Camera());




	// メインループ
	while (window.ProcessMessage())
	{

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