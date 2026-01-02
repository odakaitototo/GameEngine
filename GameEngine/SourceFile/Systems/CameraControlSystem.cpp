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

#include "Core/Input.h"

extern Input gInput;

void CameraControlSystem::Update(Coordinator* coordinator, float deltaTime)
{
	// 登録されているすべてのカメラ（通常は1つ）に対して処理
	for (auto const& entity : m_entities)
	{

		// Transform（位置）を取得
		auto& transform = coordinator->GetComponent<Transform>(entity);

		float moveSpeed = speed * deltaTime;

		// --- キー入力チェック ---
		// Windowsの機能を使って、キーが押されているか直接確認します
		// 0x8000は現在押されているというフラグです

		// Qキー：前へ（Zプラス）※カメラの向きによって変わりますが、今は単純にZ軸移動
		if (gInput.GetKey('Q'))
		{
			transform.Position.z += moveSpeed;
		}

		// Eキー：後ろへ（Zマイナス）
		if (gInput.GetKey('E'))
		{
			transform.Position.z -= moveSpeed;
		}

		// Dキー：右へ（Xプラス）
		if (gInput.GetKey('D'))
		{
			transform.Position.x -= moveSpeed;
		}

		// Aキー：左へ（Xマイナス）
		if (gInput.GetKey('A'))
		{
			transform.Position.x += moveSpeed;
		}

		// Sキー：下降」（Yプラス）
		if (gInput.GetKey('S'))
		{
			transform.Position.y += moveSpeed;
		}

		// Wキー：上昇（Yマイナス）
		if (gInput.GetKey('W'))
		{
			transform.Position.y -= moveSpeed;
		}

	}
}