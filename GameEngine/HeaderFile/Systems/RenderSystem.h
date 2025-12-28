////////////////////////////////
// 
// RenderSystem.h
// 
// 役割：MeshとTransformを持つEntityを自動的に見つけて描画するシステム
// 
// 作成開始日：2025/12/20
// 
// 作成日：2025/
// 
// 
// 
// 
// 
////////////////////////////////

////////////////////----- 単語帳 -----////////////////////////////////////////////////////////////////////////////////
//
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <d3d11.h>
#include <map>
#include <DirectXMath.h>

#include "ECS/SystemManager.h" // 親クラス Systemを使うため
#include "Graphics/DX11Device.h" // 描画命令を出すため

using namespace DirectX;

// 定数バッファ用データの定義
// シェーダーの　cbuffer と中身を一致させる必要がある
struct ConstantBufferData
{
	XMMATRIX World; // ワールド行列
	XMMATRIX View; // ビュー行列
	XMMATRIX Projection; // プロジェクション行列
};

class Coordinator; // 「Coordinatorというクラスが存在しますよ」という予告（前方宣言）


// Systemクラスを継承して作る
class RenderSystem : public System
{
public:

	// 初期値：シェーダーの読み込み等を行う
	// 引数で dx11Device をもらうのは、シェーダー作成機能を使うため
	void Init(DX11Device* device);

	// 描画実行：マイフレーム呼ばれる
	void Render(Coordinator* coordinator, Entity cameraEntity);

	// 終了処理
	void Shutdown();

private:
	DX11Device* m_device; // 描画機能へのポインタ
	ID3D11DeviceContext* m_context; // 描画コマンド発行用

	// シェーダー関連（Main.cppから引っ越してくる）
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_inputLayout;


	ID3D11Buffer* m_constantBuffer;

	// 各Entityごとの頂点バッファを管理する地図
	// キー：Entity, ID, 値：そのEntity専用の頂点バッファ
	std::map<Entity, ID3D11Buffer*> m_vertexBuffers;
};