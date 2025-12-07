////////////////////////////////
// 
// Main.cpp
// 作成開始日：2025/12/7
// 
// 
// 
// 
// 
////////////////////////////////




#include <Windows.h>
#include "HeaderFile/Core/Window.h"
#include "HeaderFile/Core/Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window;

	// 初期化
	if (!window.Init(hInstance, SCREEEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITL))
	{
		return -1;
	}

	// メインループ
	while (window.ProcessMessage())
	{
		// ここに将来以下の処理が入ります
		// Game.Update();
		// Game.Render();
	}

	return 0;
}