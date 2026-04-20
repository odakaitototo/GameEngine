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


#pragma once
#include "Core//Window.h"
#include "Graphics//DX11Device.h"
#include "ECS//Coordinator.h"
#include <memory>

class Application {
public:
	Application();
	~Application();

	// エンジンの「開始」から「」終了までを管理するメイン関数
	void Run();

private:
	// 内部的な仕事
	void Initialize(); // 初期化
	void Update(float dt); // 更新
	void Render(); // 描画

	std::unique_ptr<Window>m_window;
	std::unique_ptr<DX11Device>m_dx11Device;
	std::unique_ptr<Coordinator>m_coordinator;

	

};
