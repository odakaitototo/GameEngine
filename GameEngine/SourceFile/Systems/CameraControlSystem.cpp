////////////////////////////////
// 
// CameraControlSystem.cpp
// 
// 役割：WASDでTransformを書き換える実装
// 
// 作成開始日：2025/12/29
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
#include "Systems/CameraControlSystem.h"
#include "ECS/Coordinator.h"
#include "Components/Components.h"

void CameraControlSystem::Update(Coordinator* coordinator)
{
	// 登録されているすべてのカメラ（通常は1つ）に対して処理
	for (auto const& entity : m_entities)
	{

		// Transform（位置）を取得
		auto& transform = coordinator->GetComponent<Transform>(entity);

		// --- キー入力チェック ---
		// Windowsの機能を使って、キーが押されているか直接確認します
		// 0x8000は現在押されているというフラグです

		// Qキー：前へ（Zプラス）※カメラの向きによって変わりますが、今は単純にZ軸移動
		if (GetAsyncKeyState('Q') & 0x8000)
		{
			transform.Position.z += speed;
		}

		// Eキー：後ろへ（Zマイナス）
		if (GetAsyncKeyState('E') & 0x8000)
		{
			transform.Position.z -= speed;
		}

		// Dキー：右へ（Xプラス）
		if (GetAsyncKeyState('D') & 0x8000)
		{
			transform.Position.x += speed;
		}

		// Aキー：左へ（Xマイナス）
		if (GetAsyncKeyState('A') & 0x8000)
		{
			transform.Position.x -= speed;
		}

		// Sキー：下降」（Yプラス）
		if (GetAsyncKeyState('S') & 0x8000)
		{
			transform.Position.y += speed;
		}

		// Wキー：上昇（Yマイナス）
		if (GetAsyncKeyState('W') & 0x8000)
		{
			transform.Position.y -= speed;
		}

	}
}