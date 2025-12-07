///////////////////////////////////////////////////////////////////////////////////////////////////
// 
// DX11Device.h
// 
// 役割：GPUを操作するための（Device（GPUそのもの）,Context（描画命令を出す存在）,SwapChain（画面の表と裏を管理する存在））
// 　　　と描画先の画用紙（RenderTargetView（描画先の画用紙））を管理
// 
// 作成開始日：2025/12/7
// 
// 
// 
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////----- 単語帳 -----////////////////////////////
//
// ・GPU：画像処理、描画の専門家。
// 
// 
///////////////////////////////////////////////////////////////////

#pragma once
#include "d3d11.h"

// ライブラリのリンク指定（プロパティ設定する代わり）
#pragma comment(lib, "d3d11.lib")


class DX11Device
{
public:
	DX11Device();
	~DX11Device();

	// DirectXの初期化
	// hwnd: ウィンドウハンドル（描画先）
	bool Init(HWND hwnd, int width, int height);

	// 画面をクリアして、描画を開始する準備をする
	void Begin(float r, float g, float b);

	// 描画内筒を画面に反映する（フリップ）
	void End();

	// デバイス取得（他のクラスから呼ぶよう）
	ID3D11Device* GetDevice() const { return m_device; }
	ID3D11DeviceContext* GetContext() const { return m_context; }

private:
	// DirectX の主要オブジェクト
	ID3D11Device* m_device; // GPUそのもの
	ID3D11DeviceContext* m_context; // 描画命令を出す存在
	IDXGISwapChain* m_swapChain; // 画面の表と裏を管理する存在
	ID3D11RenderTargetView* m_rtv; // 描画先の画用紙（バックバッファ）
};
