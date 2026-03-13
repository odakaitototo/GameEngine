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
// 作成日：2025/02/10
// 作業内容：#6
//       追加：ジオメトリ生成関数の追加（立方体の呼び出し）
// 
////////////////////////////////




#include <Windows.h>
#include "Core/Window.h"
#include "Core/Main.h"
#include "Graphics/DX11Device.h" 
#include "Graphics/GeometryGenerator.h" 

// #3:ECS関連
#include "ECS/Coordinator.h"
#include "Components/Components.h"
#include "Systems/RenderSystem.h"

// システム・コア関連
#include "Systems/CameraControlSystem.h"
#include "Core/GameTimer.h"
#include "Core/Input.h"
#include "Core/OBJLoader.h"
#include "Systems/RotationSystem.h" // RotationSystemの綴りを修正

// グローバル変数
Coordinator gCoordinator;
Input gInput;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    Window window;
    DX11Device dx11;

    if (!window.Init(hInstance, SCREEEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE))
    {
        return -1;
    }

    if (!dx11.Init(window.GetHandle(), SCREEEN_WIDTH, SCREEN_HEIGHT))
    {
        MessageBox(NULL, L"DirectX Init Failed", L"Error", MB_OK);
        return -1;
    }

    gCoordinator.Init();
    gInput.Init();

    GameTimer timer;
    timer.Reset();

    // コンポーネントの登録
    gCoordinator.RegisterComponent<Transform>();
    gCoordinator.RegisterComponent<Mesh>();
    gCoordinator.RegisterComponent<Camera>();

    // システムの登録
    auto renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
    auto cameraControlSystem = gCoordinator.RegisterSystem<CameraControlSystem>();
    auto rotationSystem = gCoordinator.RegisterSystem<RotationSystem>();

    // システムシグネチャの設定
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Transform>());
        signature.set(gCoordinator.GetComponentType<Camera>());
        gCoordinator.SetSystemSignature<CameraControlSystem>(signature);
    }
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Transform>());
        signature.set(gCoordinator.GetComponentType<Mesh>());
        gCoordinator.SetSystemSignature<RenderSystem>(signature);
    }
    // ※RotationSystemのシグネチャも必要であればここに追加してください

    renderSystem->Init(&dx11);

    // OBJ読み込み
    std::vector<Vertex> meshVertices;
    // パスはご自身の環境に合わせて調整してください
    if (!OBJLoader::Load("C:\\cube.obj", meshVertices))
    {
        // 読み込み失敗時、とりあえず手動で1枚の三角形を作ってエラーを回避する
        // 座標(3), 色(4), 法線(3) の計10個の値を並べます
        meshVertices = {
            {  0.0f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f },
            {  0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f },
            { -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f }
        };
    }

    // Entity作成
    for (int i = 0; i < 5; ++i)
    {
        Entity box = gCoordinator.CreateEntity();
        Transform trans;
        trans.Position = XMFLOAT3((float)(i - 2) * 2.5f, 0.0f, 0.0f);
        trans.Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
        trans.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
        gCoordinator.AddComponent(box, trans);

        Mesh mesh;
        mesh.Vertices = meshVertices;
        gCoordinator.AddComponent(box, mesh);
    }

    // カメラEntity作成
    Entity cameraEntity = gCoordinator.CreateEntity();
    Transform cameraTransform;
    cameraTransform.Position = XMFLOAT3(0.0f, 0.0f, -5.0f);
    gCoordinator.AddComponent(cameraEntity, cameraTransform);

    Camera cameraComp;
    cameraComp.AspectRatio = (float)SCREEEN_WIDTH / (float)SCREEN_HEIGHT;
    gCoordinator.AddComponent(cameraEntity, cameraComp);

    // メインループ
    while (window.ProcessMessage())
    {
        timer.Tick();
        float dt = timer.DeltaTime();

        gInput.Update();
        cameraControlSystem->Update(&gCoordinator, dt);
        rotationSystem->Update(&gCoordinator, dt);

        dx11.Begin(0.1f, 0.2f, 0.1f);
        renderSystem->Render(&gCoordinator, cameraEntity);
        dx11.End();
    }

    renderSystem->Shutdown();
    return 0;
}