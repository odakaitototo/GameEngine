///////////////////////////////////////////////////////////////////////////////////////////////////
// 
// EntityManager.h
// 
// 役割：Entity（ID）を管理するクラス
// 
// 製作開始日：2025/12/8
// 作成日：2025/12/10
// 作業内容：#1
//       追加：m_signatures[entity].reset();の追加
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
		assert(mLivingEntityCount < MAX_ENTITIES && "TOO many entities in existence"); // 現在のEntityの数が最大数を超えていないかの確認

		// キューの先頭からIDを取り出す
		Entity id = m_availableEntities.front(); // 未使用のEntityIDのキューの先頭からIDを取得する

		m_availableEntities.pop(); /* 取り出したIDをm_availableEntitiesから消去する。
		                              これにより同じIDを他の場所で使用できないようにする*/

		++mLivingEntityCount; // 使用しているエンティティの数を1つ増やす

		return id; // 取得したIDを呼び出し先に返す
	}
	 // Entityの破棄（IDを回収する）
	void DestroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range."); // 回収しようとしているIDがありえない数値じゃないかチェック
		
		
		m_signatures[entity].reset(); // #1:IDが戻ってきたらシグネチャ（持っているコンポーネント情報）をリセット

		m_availableEntities.push(entity); //使い終わったIDを再びm_availableEntities（空き番箱(キュー)）の一番後ろに入れる 

		--mLivingEntityCount; // 使用中のEntityボックスの中の数値を1つ減らす
	}

	void SetSignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		m_signatures[entity] = signature;
	}


	// Entityのシグネチャを取得（システムが対象かどうかを判定するときに使う）
	Signature GetSignature(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		return m_signatures[entity]; // 配列から、そのIDに対応するシグネチャを取り出して返す
	}

private:

	
	queue<Entity> m_availableEntities; // 未使用のEntity IDを入れておくキュー

	/* 各Entityがどのコンポーネントを持っているか記録する配列
	   インデックスがEntity IDに対応*/
	array<Signature, MAX_ENTITIES> m_signatures;

	// 現在生存しているEntity数
	uint32_t mLivingEntityCount = 0;

};
