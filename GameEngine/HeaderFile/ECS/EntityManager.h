///////////////////////////////////////////////////////////////////////////////////////////////////
// 
// EntityManager.h
// 
// 役割：Entity（ID）を管理するクラス
// 
// 製作開始日：2025/12/8
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
#include "ECSTypes.h"
#include <queue>
#include <array>
#include <cassert>


class EntityManager
{
public:
	EntityManager()
	{
		// 全てのID（0～MAX-1）を「使用可能キュー」に入れておく
		for (Entity e = 0; e < MAX_ENTITIES; ++e)
		{
			m_availableEntities.push(e);
		}
	}

	// 新しいEntityを作成（IDを払いだす）
	Entity CreateEntity()
	{
		assert(mLivingEntityCount < MAX_ENTITIES && "TOO many entities in existence");

		// キューの先頭からIDを取り出す
		Entity id = m_availableEntities.front();
		m_availableEntities.pop();
		++mLivingEntityCount;

		return id;
	}
	 // Entityの破棄（IDを回収する）
	void DestroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		
		// IDが戻ってきたらシグネチャ（持っているコンポーネント情報）をリセット
		m_availableEntities.push(entity);
		--mLivingEntityCount;
	}

	// Entityのシグネチャを取得（システムが大賞かどうかを判定するときに使う）
	Signature GetSignature(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		return m_signatures[entity];
	}

private:

	// 未使用のEntity IDを入れておくキュー
	queue<Entity> m_availableEntities;

	// 各Entityがどのコンポーネントを持っているか記録する配列
	// インデックスがEntity IDに対応
	array<Signature, MAX_ENTITIES> m_signatures;

	// 現在生存しているEntity数
	uint32_t mLivingEntityCount = 0;

};
