////////////////////////////////
// 
// RenderSystem.cpp
// 
// 役割：RenderSystemクラス定義
// 
// 作成開始日：2025/12/21
// 
// 作成日：2025/12/23
// 　　作業内容：#1
// 　　　　　追加：
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

#include "Systems/RenderSystem.h"
#include "ECS/Coordinator.h"
#include "Components/Components.h"

// 安全に解放するマクロ
template<typename T>
void SafeRelease(T*& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

void RenderSystem::Init(DX11Device* device)
{
	m_device = device;
	m_context = device->GetContext();

	// ---シェーダーの読み込み---
	ID3DBlob* vsBlob = nullptr;
	m_device->CreateVertexShader(L"SimpleVertexShader.cso", &m_vertexShader, &vsBlob);
	m_device->CreatePixelShader(L"SimplePixelShader.cso", &m_pixelShader);

	// ---InputLayoutの作成---
	m_device->CreateInputLayout(vsBlob, &m_inputLayout);
	
	// Blobはもう不要
	SafeRelease(vsBlob);

	// #1:定数バッファの作成
	D3D11_BUFFER_DESC cbd = { 0 };
	cbd.ByteWidth = sizeof(ConstantBufferData); // バッファサイズ
	cbd.Usage = D3D11_USAGE_DEFAULT; // CPUから書き換える標準的な設定
	cbd.BindFlags = D3D10_BIND_CONSTANT_BUFFER; // 「これは定数バッファです」
	cbd.CPUAccessFlags = 0;

	// デバイスを使ってバッファ作成（初期化データなしでOK）
	m_device->GetDevice()->CreateBuffer(&cbd, nullptr, &m_constantBuffer);
}

void RenderSystem::Render(Coordinator* coordinator)
{
	// 描画の共通設定（シェーダーやレイアウトなど）
	// 同じシェーダーを使うなら、ループの外で1回セットすれば効率がいい
	m_context->IASetInputLayout(m_inputLayout);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_context->VSSetShader(m_vertexShader, nullptr, 0);
	m_context->PSSetShader(m_pixelShader, nullptr, 0);

	m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);// #1:シェーダーの定数バッファスロット0番にセット

	// #1:カメラ行列の計算（View,Projection）
	// 本来はCameraSystem等で管理しますが、今は子オデ簡易作成します

	// #1:ビュー行列：カメラの向きと位置
	XMVECTOR eyePos = XMVectorSet(0.0f, 0.0f, -2.0f, 0.0f); // カメラは手前(Z=-2)に置く
	XMVECTOR focusPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // 原点を見る
	XMVECTOR upDir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // 上方向はY軸
	XMMATRIX viewMatrix = XMMatrixLookAtLH(eyePos, focusPos, upDir);

	// #1:プロジェクション行列：遠近感の設定
	// 画角60度、アスペクト比16：9、手前0.1f～奥100.0fまで見える
	XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

	// ---Entityごとの描画ループ---
	// 三角形を回転させるために、時間を取得（簡易的なカウンター）
	static float time = 0.0f;
	time += 0.005f; // #1:毎フレーム少しずつ増やす

// m_entitiesには「MeshとTransformを持ってるEntity」だけが自動的に入っている
	for (auto const& entity : m_entities)
	{
		// そのEntityのデータ（Component）を取得
		auto& transform = coordinator->GetComponent<Transform>(entity);
		auto& mesh = coordinator->GetComponent<Mesh>(entity);

		// #1:ワールド行列の計算(World)

		// #1:回転（Y軸回転）：timeを使ってクルクル回す
		XMMATRIX worldMatrix = XMMatrixRotationY(time);

		// #1:もしEntityの位置(transform.Positino)を使いたければこう合成いします：
		// worldMatrix = worldMatrix * XMMatrixTranslation(transform.Position.x,...);

		// #1:定数バッファの更新
		ConstantBufferData cb;
		// #1:DirectXMathの行列は転置（Transpose）してGPUに送るルールがあります
		cb.World = XMMatrixTranspose(worldMatrix);
		cb.View = XMMatrixTranspose(viewMatrix);
		cb.Projection = XMMatrixTranspose(projMatrix);

		m_context->UpdateSubresource(m_constantBuffer, 0, nullptr, &cb, 0, 0); // #1:GPUのメモリを更新

		// 頂点バッファの準備
		// 「このEntity用のバッファはもう作ったことある？」と確認
		if (m_vertexBuffers.find(entity) == m_vertexBuffers.end())
		{
			// まだないなら作る（初回のみ実装される：遅延初期化）
			ID3D11Buffer* buffer = nullptr;

			// Meshコンポーネントには入っている頂点データを使ってバッファ作成
			// vectorの先頭ポインタは ,data()でとれる
			m_device->CreateVertexBuffer(mesh.Vertices.data(), (UINT)(sizeof(Vertex) * mesh.Vertices.size()), &buffer);

			// 地図に登録
			m_vertexBuffers[entity] = buffer;
		}

		// 描画コマンド発行
		ID3D11Buffer* vBuffer = m_vertexBuffers[entity];
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		// 頂点バッファをセット
		m_context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);

		// 描画（頂点の数だけ書く）
		m_context->Draw((UINT)mesh.Vertices.size(), 0);
	}
}


void RenderSystem::Shutdown()
{
	SafeRelease(m_constantBuffer);
	// 作ったバッファを全消去
	for (auto const& pair : m_vertexBuffers)
	{
		pair.second->Release();
	}
	m_vertexBuffers.clear();

	SafeRelease(m_inputLayout);
	SafeRelease(m_pixelShader);
	SafeRelease(m_vertexShader);
}

