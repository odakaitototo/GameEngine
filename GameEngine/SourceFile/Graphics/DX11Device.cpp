///////////////////////////////////////////////////////////////////////////////////////////////////
// 
// DX11Device.cpp
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

////////////////////////////////////////////----- 単語帳 -----/////////////////////////////////////////////////
//
// ・Release();にしている理由：「使い終わりました」という報告。カウントが0になったら自動消去する仕組み。
// 　　　　　　　　　　　　　　 Deleteにすると別の場所で使っている場合使い終わっていない場所も消してしまうから。
// 
// ・作った順と逆の順番で解放する理由：上から解放すると土台がなくなった様な状態になりその後の解放で
// 　　　　　　　　　　　　　　　　　　エラーが起きる恐れがあるから。
// 
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "Graphics/DX11Device.h"

//////////////////
// コンストラクタ
//////////////////
DX11Device::DX11Device()
	:m_device(nullptr), 
	 m_context(nullptr), 
	 m_swapChain(nullptr), 
	 m_rtv(nullptr)
{
}

//////////////////
// デストラクタ
//////////////////
DX11Device::~DX11Device()
{
	// 作った順と逆の順番で解放するのが作法
	if (m_rtv) m_rtv->Release();
	if (m_swapChain) m_swapChain->Release();
	if (m_context) m_context->Release();
	if (m_device) m_device->Release();
}

//////////////////
// 初期化処理
//////////////////
bool DX11Device::Init(HWND hwnd, int width, int height)
{
	HRESULT hr;

	// スワップチェーンの設定（画面設定）
	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferCount = 1; // バッファの数
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色の形式
	scd.BufferDesc.RefreshRate.Numerator = 60; // リフレッシュレート 60fps
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 描画ターゲットとして使う
	scd.OutputWindow = hwnd; // 描画するウィンドウ
	scd.SampleDesc.Count = 1; // アンチエイリアスなし
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE; // ウィンドウモード

	// デバイスとスワップチェーンの作成
	// D3D_DRIVER_TYPE_HARDWARE：グラボを使う設定
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		nullptr, 0, D3D11_SDK_VERSION, &scd,
		&m_swapChain, &m_device, nullptr, &m_context
	);

	if (FAILED(hr)) return false;

	// バックバッファ（描画領域）の取得とRTVの作成
	ID3D11Texture2D* backBuffer = nullptr;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(hr)) return false;

	hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_rtv);
	backBuffer->Release(); // RTVさえ作れれば画像ポインタは不要
	if (FAILED(hr)) return false;

	// 描画先をバックバッファに設定
	m_context->OMSetRenderTargets(1, &m_rtv, nullptr);

	// ビューポート（描画範囲）の設定
	D3D11_VIEWPORT vp = { 0 };
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_context->RSSetViewports(1, &vp);

	return true;
	
}

void DX11Device::Begin(float r, float g, float b)
{
	float clearColor[4] = { r, g, b, 1.0f }; // R,G,B,A  裏画面を指定色で塗りつぶしている
	// 画面を指定職でクリア（塗りつぶし）
	m_context->ClearRenderTargetView(m_rtv, clearColor);
}

//////////////////
// 終了処理
//////////////////
void DX11Device::End()
{
	// 裏画面を表画面に入れ替え（VSync有効）
	m_swapChain->Present(1, 0);
}
