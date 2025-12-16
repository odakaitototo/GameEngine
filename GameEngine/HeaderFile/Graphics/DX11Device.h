///////////////////////////////////////////////////////////////////////////////////////////////////
// 
// DX11Device.h
// 
// 役割：GPUを操作するための（Device（GPUそのもの）,Context（描画命令を出す存在）,SwapChain（画面の表と裏を管理する存在））
// 　　　と描画先の画用紙（RenderTargetView（描画先の画用紙））を管理
// 
// 作成開始日：2025/12/7
// 
// 作成日：2025/12/11
// 作業内容：#1
//       追加：#includeと#pragmaの追加
// 　　　追加：シェーダーを作成する関数の追加
// 
// 作成日：2025/12/13
// 作業内容：#2
// 　　　追加：頂点構造体の定義
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
#include <d3dcompiler.h> // #1:シェーダー読み込み用
#include <DirectXMath.h> // #2:DirectX用の数学ライブラリの追加


#pragma comment(lib, "d3d11.lib") // ライブラリのリンク指定（プロパティ設定する代わり）
#pragma comment(lib, "d3dcompiler.lib") // #1:これがないとリンクエラーになります

using namespace DirectX; // XMFLOAT3を使うため

/* #2:頂点データの型定義
   シェーダーのVS_INPUT構造体と意味を合わせる必要がある*/
struct Vertex
{
	float x, y, z; // #2:位置座標
};


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



public:
	/* #1:シェーダーを作成する関数
	   filename:読み込むファイル名（L"SimpleVertexShader.cso"など）*/
	bool CreateVertexShader(const wchar_t* filename, ID3D11VertexShader** outShader, ID3DBlob** outBlob);
	bool CreatePixelShader(const wchar_t* filename, ID3D11PixelShader** outShader);

private:
	// メンバ変数として持っておく必要がないかもしれないが、
	// 将来「作成済みシェーダー」を管理したくなるので、今は作成関数だけ作ります。


public:
	/* #2:頂点バッファを作成する
	   data:頂点データの配列の先頭ポインタ
	   size:配列全体のサイズ（バイト数）
	   outBuffer:作成されたバッファを受け取るポインタ*/
	bool CreateVertexBuffer (const Vertex* data, UINT size, ID3D11Buffer** outbuffer);

	/* #2:インプットレイアウトを作成する
	　　vsBlob:頂点シェーダーのコンパイル済みデータ（これと照合するために必要）
	  　outLayout:作成されたレイアウトを受け取るポインタ*/
	bool CreateInputLayout(ID3DBlob* vsBlob, ID3D11InputLayout** outlayout);

};
