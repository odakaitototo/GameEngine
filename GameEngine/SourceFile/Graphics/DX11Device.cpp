///////////////////////////////////////////////////////////////////////////////////////////////////
// 
// DX11Device.cpp
// 
// 役割：GPUを操作するための（Device（GPUそのもの）,Context（描画命令を出す存在）,SwapChain（画面の表と裏を管理する存在））
// 　　　と描画先の画用紙（RenderTargetView（描画先の画用紙））を管理
// 
// 作成開始日：2025/12/7
// 
// 作成日：2025/12/13
// 作業内容：#1
// 　　　追加：シェーダーを読み込む機能の実装
// 
// 作成日：2025/12/13
// 作業内容：#2
// 　　　追加：頂点バッファとインプットレイアウトの作成
// 
// 作成日：2025/01/03
// 作成内容：#3
// 　　　追加：法線データの追加
// 
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

// #1：頂点シェーダー作成
bool DX11Device::CreateVertexShader(const wchar_t* filename, ID3D11VertexShader** outShader, ID3DBlob** outBlob)
{
	ID3DBlob* blob = nullptr;
	HRESULT hr;

	// #1：.csoファイルをバイナリ（blob）として読み込む
	hr = D3DReadFileToBlob(filename, &blob);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Shader File Not Found!", filename, MB_OK);
		return false;
	}

	// #1：Blobデータを使って、GPU用の頂点シェーダーオブジェクトを作成
	hr = m_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, outShader);

	if (FAILED(hr))
	{
		blob->Release();
		return false;
	}

	/* #1：頂点シェーダの場合、後でInputLayoutを作る時にこのBlobが必要になるので、
	 呼び出し元にBlobを返す（呼び出しもとでReleaseしてもらう）*/
	if (outBlob)
	{
		*outBlob = blob;
	}
	else
	{
		blob->Release(); //#1：不要ならここで解放
	}
	return true;
}

// #1：ピクセルシェーダー作成
bool DX11Device::CreatePixelShader(const wchar_t* filename, ID3D11PixelShader** outShader)
{
	ID3DBlob* blob = nullptr;
	HRESULT hr;

	// #1：ファイル読み込み
	hr = D3DReadFileToBlob(filename, &blob);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Shader File Not Found!", filename, MB_OK);
		return false;
	}

	// #1：ピクセルシェーダー作成
	hr = m_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, outShader);

	blob->Release(); // #1：ピクセルシェーダーは作成さえすればBlobはもう不要

	if (FAILED(hr))
	{
		return false;
	}

	return true;
	
}
// #2:頂点バッファ作成
bool DX11Device::CreateVertexBuffer(const Vertex* data, UINT size, ID3D11Buffer** outBuffer)
{
	// #2:バッファの設計図を作る
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT; // #2:GPUが読み書きする標準的なメモリ
	bd.ByteWidth = size; // #2:バッファのサイズ（バイト数）
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // #2:「これは頂点バッファとして使います
	bd.CPUAccessFlags = 0; // #2:CPUからはアクセスしない（高速化のため）

	// #2:バッファに入れる「初期データ」を使用する
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = data; // #2:コピー元のデータ（CPU側の配列）

	// #2:設計図とデータを使って、GPUメモリ上にバッファを作成
	HRESULT hr = m_device->CreateBuffer(&bd, &initData, outBuffer);

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Create VertexBuffer Failed", L"Error", MB_OK);
		return false;
	}

	return true;

}
// #2:インプットレイアウト作成
bool DX11Device::CreateInputLayout(ID3DBlob* vsBlob, ID3D11InputLayout** outLayout)
{
	/* #2:レイアウトの定義配列
	  「c++の構造体のこの部分」が「シェーダーのこのセマンティクス」に対応する、
	   という対応表*/
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// SemanticName, SemanticIndex, Format, InputSlot, AlignedByteOffset,

        // 1つ目：位置情報
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
		},

		// #2:2つ目：色情報
		// AlignedByteOffsetが12なのは、前のデータ(float3つ分 = 4byte*3)の後ろにあるから
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0
        },

		// #3:法線（NORMAL）を追加
		//　前のデータ（pos:12byte + color:16byte）の後ろにある
		{
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0
        },
	};
	
	/* 解説：
	   "POSITION" -> シェーダー側の：POSITIONと対応させる
	   DXGI_FORMAT_R32G32B32_FLOAT -> floatが3つ（x, y, z）並んでいるデータだよ、という説明
	   AlignedByteOffset -> 先頭から何バイト目にあるか（今回は先頭なので0）*/

	UINT numElements = ARRAYSIZE(layout);

	/* レイアウトオブジェクトを作成
	   ここで「頂点シェーダーの入力定義(vsBlob)」と「上記のレイアウト定義」が
	   矛盾していないか（型などが合っているか）チェックされます*/
	HRESULT hr = m_device->CreateInputLayout(
		layout,
		numElements,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		outLayout
	);


	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Create InputLayout Failed", L"Error", MB_OK);

		return false;
	}

	return true;
}
