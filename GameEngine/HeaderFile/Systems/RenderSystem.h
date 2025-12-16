///////////////////////////////////////////////////////////////////////////////////////////////////
// 
// TrRenderSystem.h
// 
// 役割：TransformとMeshを持つEntityを描画する専門職人
// 作成開始日：2025/12/16
// 
// 作成日：
// 作業内容：#1
//       追加：
// 　　　追加：
// 
// 
// 
// 
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////----- 単語帳 -----////////////////////////////
//
// ・
// 
// 
///////////////////////////////////////////////////////////////////
#pragma once
#include "ECS/SystemManager.h"
#include "ECS/Coordinator.h" // コンポーネントを取得するために必要
#include "Graphics//DX11Device.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"


class RenderSystem : public System
{
public:
	/*描画実行関数
	　毎フレームMain.cpp から呼ばれます
	  device: 各Entityのコンポーネントデータを取り出すために必要*/
	void Render(DX11Device* device, Coordinator* coordinator)
	{
		auto context = device->GetContext();// 詳しく

		// 管理している全てのEntity（TransformとMwshを持っているやつら）をループ処理
		for (auto const& entity : m_entities)
		{
			// そのEntityが持っているコンポーネントを取得
			auto& transform = coordinator->GetComponent<TransformComponent>(entity);
			auto& mesh = coordinator->GetComponent<MeshComponent>(entity);
		}
	}
};