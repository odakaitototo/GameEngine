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
// 作成日：2025/12/30
// 作業内容：#3
// 　　　追加：色の追加
// 
// 作成日：2025/01/03
// 作業内容：#4
//       追加：法線の追加
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
#include <d3d11.h>
#include <d3dcompiler.h> // #1:シェーダー読み込み用
#include <DirectXMath.h> // #2:DirectX用の数学ライブラリの追加

#include "../../HeaderFile/Components/Components.h"

#pragma comment(lib, "d3d11.lib") // ライブラリのリンク指定
#pragma comment(lib, "d3dcompiler.lib") // #1:これがないとリンクエラーになります

using namespace DirectX; // XMFLOAT3を使うため

/* #2:頂点データの型定義
   シェーダーのVS_INPUT構造体と意味を合わせる必要がある */


class DX11Device
{
public:
    DX11Device();
    ~DX11Device();

    // DirectXの初期化
    bool Init(HWND hwnd, int width, int height);

    // 画面をクリアして、描画を開始する準備をする
    void Begin(float r, float g, float b);

    // 描画内容を画面に反映する（フリップ）
    void End();

    // デバイス取得
    ID3D11Device* GetDevice() const { return m_device; }
    ID3D11DeviceContext* GetContext() const { return m_context; }

    // #1:シェーダーを作成する関数
    bool CreateVertexShader(const wchar_t* filename, ID3D11VertexShader** outShader, ID3DBlob** outBlob);
    bool CreatePixelShader(const wchar_t* filename, ID3D11PixelShader** outShader);

    // #2:頂点バッファを作成する
    bool CreateVertexBuffer(const Vertex* data, UINT size, ID3D11Buffer** outbuffer);

    // #2:インプットレイアウトを作成する
    bool CreateInputLayout(ID3DBlob* vsBlob, ID3D11InputLayout** outlayout);

private:
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;
    IDXGISwapChain* m_swapChain = nullptr;
    ID3D11RenderTargetView* m_rtv = nullptr;
};