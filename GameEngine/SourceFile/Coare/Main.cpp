#include <Windows.h>
#include "HeaderFile/Core/Window.h"
#include "HeaderFile/Core/Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window;

	// ‰Šú‰»
	if (!window.Init(hInstance, SCREEEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITL))
	{
		return -1;
	}

	// ƒƒCƒ“ƒ‹[ƒv
	while (window.ProcessMessage())
	{
		// ‚±‚±‚É«—ˆˆÈ‰º‚Ìˆ—‚ª“ü‚è‚Ü‚·
		// Game.Update();
		// Game.Render();
	}

	return 0;
}