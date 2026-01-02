////////////////////////////////
// 
// Rotation.h
// 
// 役割：Transform（位置情報）を持っている全員」を対象にして、毎フレーム少しずつ角度を変える役割
// 
// 作成開始日：2025/1/3
// 
// 作成日：2025/1/
// 　　
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

#include "ECS/SystemManager.h"
#include "ECS/Coordinator.h"
#include "Components/Components.h"

class RotationSystem : public System
{
public:
	// マイフレーム回転させる処理
	void Update(Coordinator* coordinator, float deltaTime)
	{
		for (auto const& entity : m_entities)
		{
			// Transformコンポーネントを取得
			auto& transform = coordinator->GetComponent<Transform>(entity);

			// Y軸(縦軸)を中心にクルクル回す
			// 1秒間に90度(PI / 2)回転するスピード
			float speed = 1.57f;

			transform.Rotation.y += speed * deltaTime;

			// 360度を超えたら0に戻す（オーバーフロー防止）
			if (transform.Rotation.y > 6.28f)
			{
				transform.Rotation.y -= 6.28;
			}
		}
	}
};