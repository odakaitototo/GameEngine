////////////////////////////////
// 
// GameTimer.h
// 
// 役割：関数のリスト
// 
// 作成開始日：2026/3/18
// 
// 作成日：2026/3/18
// 　　作業内容：
// 　　　　　追加：
// 
// 
// 
// 
// 
////////////////////////////////

////////////////////----- 単語帳 -----////////////////////////////////////////////////////////////////////////////////
//
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Core//Aplication.h"

Application::Application() 
{
	Initialize();
}

Application::~Application()
{

}

void Application::Initialize()
{
	// 今Main.cppにあるウィンドウ作成やDirectX初期化をここに移動させる
	m_window = std::make_unique<Window>(/*1280, 720, L"Project: Rescue Trace"*/);
	m_dx11Device = std::make_unique<DX11Device>(/*m_window->GetHandle()*/);
	m_coordinator = std::make_unique<Coordinator>();
	m_coordinator->Init();

	// ここでコンポーネントやシステムの登録も行うようにします
}

void Application::Run()
{
	// メッセージループ（ゲームが動いている間のループ）
	while(m_window->ProcessMessage())
	{
		Update(0.016f);
		Render();
	}
}


void Application::Update(float dt)
{
	// システムの更新処理をここにまとめる
}

void Application::Render()
{
	// 画面のクリアと描画命令をここにまとめる
}