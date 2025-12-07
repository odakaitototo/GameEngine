////////////////////////////////
// 
// Window.h
// 作成開始日：2025/12/7
// 
// 
// 
// 
// 
////////////////////////////////

#pragma once
#include <Windows.h>

class Window
{
public:
	Window();
	~Window();

	// ウィンドウを作成し、表示する
	// width、height:解像度、title: ウィンドウ名
	bool Init(HINSTANCE hInstance, int width, int height, const char* title);

	// メッセージを処理する（ゲーム内で毎フレーム呼ぶ）
	// 戻り値:　trueなら継続、falseなら終了
	bool ProcessMessage();

	// ウィンドウハンドルを取得（DirectX初期化で必要）
	HWND GetHandle() const { return m_hwnd; }

private:
	HWND m_hwnd; // ウィンドウハンドル
	WNDCLASSEX m_wc; // ウィンドウクラス情報
};