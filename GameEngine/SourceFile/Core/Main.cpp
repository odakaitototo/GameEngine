////////////////////////////////
// 
// Main.cpp
// 製作開始日：2025/12/07
// 
// 製作日：2025/12/07
// 作業内容：#1
// 　　　追加：Begin(画面クリア)とEnd(表示)を繰り返して描画するようにした。
// 
// 
// 
////////////////////////////////




#include <Windows.h>
#include "Core/Window.h"
#include "Core/Main.h"
#include "Graphics/DX11Device.h" // 描画するためのもの

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window;
	DX11Device dx11; // #1

	// 初期化
	if (!window.Init(hInstance, SCREEEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITL))
	{
		return -1;
	}

	// #1：DirectX初期化（ウィンドウのハンドルを渡す）
	if (!dx11.Init(window.GetHandle(), SCREEEN_WIDTH, SCREEN_HEIGHT))
	{
		MessageBox(NULL, L"DirectX Init Failed", L"Error", MB_OK);
		return -1;
	}

	// メインループ
	while (window.ProcessMessage())
	{

		// #1：描画開始（画面を濃い青色でクリア）
		dx11.Begin(0.1f, 0.2f, 0.1f); // ウィンドウの色設定


		// ここに将来以下の処理が入ります
		// Game.Update();
		// Game.Render();

		// #1：描画終了（画面を更新）
		dx11.End();
	}

	return 0;
}