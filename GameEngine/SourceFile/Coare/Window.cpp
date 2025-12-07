////////////////////////////////
// 
// Window.cpp
// 作成開始日：2025/12/7
// 
// 
// 
// 
// 
////////////////////////////////



#include "HeaderFile/Core/Window.h"


// ウィンドウプロシージャ（クラスの外に置くのが一番簡単）
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

Window::Window() : m_hwnd(nullptr), m_wc{ 0 }
{

}

Window::~Window()
{
	// 終了時に登録会よなどが必要ならここに書く
}

bool Window::Init(HINSTANCE hInstance, int width, int height, const char* title)
{

	m_wc.cbSize = sizeof(WNDCLASSEX);
	m_wc.style = CS_HREDRAW | CS_VREDRAW;
	m_wc.lpfnWndProc = WindowProc; // 上で作った関数を指定
	m_wc.hInstance = hInstance;
	m_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	m_wc.lpszClassName = "MyDX11EngineWindowClass";
	

	if (!RegisterClassEx(&m_wc))
	{
		return false;
	}
	// ウィンドウサイズの調整
	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウ生成
	m_hwnd = CreateWindowEx(
		0,
		m_wc.lpszClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!m_hwnd)
	{
		return false;
	}

	// 表示
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	return true;

}

bool Window::ProcessMessage()
{
	MSG msg = { 0 };

	// ゲームループ向け：PeekMessageを使う
	// （メッセージがない時も処理を返して、描画等を止まらせないため）
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{

	  if (msg.message == WM_QUIT)
	  {
		  return false; // アプリ終了
	  }

	     TranslateMessage(&msg);
	     DispatchMessage(&msg);

    }
	return true; // 継続

}