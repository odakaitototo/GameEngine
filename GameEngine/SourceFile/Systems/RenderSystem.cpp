////////////////////////////////
// 
// RenderSystem.cpp
// 
// 役割：RenderSystemクラス定義
// 
// 作成開始日：2025/12/21
// 
// 作成日：2025/12/28
// 　　作業内容：#1
// 　　　　　追加：カメラ行列の計算
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

void RenderSystem::Render(Coordinator* coordinator, Entity cameraEntity)
{
	// 描画の共通設定（シェーダーやレイアウトなど）
	// 同じシェーダーを使うなら、ループの外で1回セットすれば効率がいい
	m_context->IASetInputLayout(m_inputLayout);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_context->VSSetShader(m_vertexShader, nullptr, 0);
	m_context->PSSetShader(m_pixelShader, nullptr, 0);

	m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);// #1:シェーダーの定数バッファスロット0番にセット

	    /////////////////////////////////////////
		// 
		// #1:カメラ行列（View / projection）の計算
		// 
		/////////////////////////////////////////

		// カメラエンティティからコンポーネントを取得
	auto& cameraTransform = coordinator->GetComponent<Transform>(cameraEntity);
	auto& cameraComponent = coordinator->GetComponent<Camera>(cameraEntity);

	// View行列（カメラの位置、向き）の計算
	// カメラの「ワールド行列」を作って、その「逆行列」を求めるとViewf行列になります

	// 回転行列（ピッチ、ヨー、ロール）
	XMMATRIX cameraRot = XMMatrixRotationRollPitchYaw(
		cameraTransform.Rotation.x,
		cameraTransform.Rotation.y,
		cameraTransform.Rotation.z
	);
	// 並行行列移動
	XMMATRIX cameraTrans = XMMatrixTranslation(
		cameraTransform.Position.x,
		cameraTransform.Position.y,
		cameraTransform.Position.z
	);

	// カメラワールド行列 = 回転＊移動
	XMMATRIX cameraWorld = cameraRot * cameraTrans;

	// View行列 = カメラワールド行列の「逆行列(Inverse)」
	XMMATRIX viewMatrix = XMMatrixInverse(nullptr, cameraWorld);

	// Projection行列（レンズ）の計算
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(
		cameraComponent.Fov,
		cameraComponent.AspectRatio,
		cameraComponent.NearClip,
		cameraComponent.FarClip
	);

	// --- Entityごとの描画ループ ---
	static float time = 0.0f;
	time += 0.005f;

	for (auto const& entity : m_entities)
	{
		auto& transform = coordinator->GetComponent<Transform>(entity);
		auto& mesh = coordinator->GetComponent<Mesh>(entity);

		// --- ワールド行列の計算 ---
		// 今回はテスト用にY軸回転させつつ、Transformの位置も反映させる
		XMMATRIX worldMatrix = XMMatrixRotationY(time);

		// EntityのTransform位置へ移動
		 worldMatrix = XMMatrixTranslation(
			transform.Position.x,
			transform.Position.y,
			transform.Position.z
		);

		// --- 定数バッファの更新 ---
		ConstantBufferData cb;
		cb.World = XMMatrixTranspose(worldMatrix);
		cb.View = XMMatrixTranspose(viewMatrix); // さっき計算したView行列
		cb.Projection = XMMatrixTranspose(projectionMatrix); // さっき計算したProjection行列

		m_context->UpdateSubresource(m_constantBuffer, 0, nullptr, &cb, 0, 0);

		// --- 描画 ---
		if (m_vertexBuffers.find(entity) == m_vertexBuffers.end())
		{
			ID3D11Buffer* buffer = nullptr;
			m_device->CreateVertexBuffer(mesh.Vertices.data(), (UINT)(sizeof(Vertex) * mesh.Vertices.size()), &buffer);
			m_vertexBuffers[entity] = buffer;
		}
		ID3D11Buffer* vBuffer = m_vertexBuffers[entity];
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);

		m_context->Draw((UINT)mesh.Vertices.size(), 0);
	}
	
	
}


void RenderSystem::Shutdown()
{
	SafeRelease(m_constantBuffer);
	// 作ったバッファを全消去
	for (auto const& pair : m_vertexBuffers)
	{
		if (pair.second)
		{
			pair.second->Release();
		}
		
	}
	m_vertexBuffers.clear();

	SafeRelease(m_inputLayout);
	SafeRelease(m_pixelShader);
	SafeRelease(m_vertexShader);
}

