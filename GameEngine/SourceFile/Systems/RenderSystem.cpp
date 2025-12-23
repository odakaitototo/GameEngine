////////////////////////////////
// 
// RenderSystem.cpp
// 
// 役割：RenderSystemクラス定義
// 
// 作成開始日：2025/12/21
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
}

void RenderSystem::Render(Coordinator* coordinator)
{
	// 描画の共通設定（シェーダーやレイアウトなど）
	// 同じシェーダーを使うなら、ループの外で1回セットすれば効率がいい
	m_context->IASetInputLayout(m_inputLayout);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_context->VSSetShader(m_vertexShader, nullptr, 0);
	m_context->PSSetShader(m_pixelShader, nullptr, 0);

	// ---Entityごとの描画ループ---
// m_entitiesには「MeshとTransformを持ってるEntity」だけが自動的に入っている
	for (auto const& entity : m_entities)
	{
		// そのEntityのデータ（Component）を取得
		auto& transform = coordinator->GetComponent<Transform>(entity);
		auto& mesh = coordinator->GetComponent<Mesh>(entity);

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

