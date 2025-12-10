////////////////////////////////
// 
// SystemManager.h
// 役割：各種System（ロジック）の登録と・Entityリストのメンテナンスを行う場所
// 作成開始日：2025/12/10
// 
// 
// 
// 
// 
////////////////////////////////

////////////////////----- 単語帳 -----////////////////////////////
//
// ・
//
// 
///////////////////////////////////////////////////////////////////

#pragma once

#include "ECSTypes.h"
#include <set>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>

// システム規定クラス
// すべてのシステム（PhysicsSystem, RenderSystemなど）これを継承します
class System
{
public:
	// このシステムが処理すべきEntityのリスト
	set <Entity> m_entities;
};

// Systemマネージャークラス
class SystemManager
{
public:
	// システムを登録する
	template<typename T>
	shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name();
		assert(m_systems.find(typeName) == m_systems.end() && "Registering system mor than once.");

		// システムを作成して保存
		auto system = make_shared<T>();
		m_system.insert({ typeName, system });
		return system;
	}


	// システムが担当する条件（シグネチャ）を設定する
	// 例：PhysicsSystemは　「位置」と「速度」が必要
	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typename = typeid(T).name();
		assert(m_systems.find(typename) != m_systems.end() && "System used before registered.");

		m_signatures.insert({ typeName, signature });
	}

	// Entityが破棄されたとき、全システムからそのEntityを削除する
	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : m_systems)
		{
			auto const& system = pair.second;
			system->m_entities.erase(entity);
		}

	}

	// 重要：Entityのコンポーネント構成が変わった時に呼ばれる
	// そのEntityが、各システム管理下に入るべきか、外れるべきかを判定する
	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		// 登録されている全システムをチェック
		for (auto const& pair : m_systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = m_signatures[type];

			// ビット演算：Entityの持っている成分(enntitySignature)がシステムの要求(systemSignatyre)を満たしているか？
			if ((entitySignature & systemSignature) == systemSignature)
			{
				// 条件を満たすなら、リストに追加
				system->m_entities.insert(entity);
			}
			else
			{
				// 満たさないから、リストから除外
				system->m_entities.erase(entity);
			}

		}
	}

private:
	// 型名 -> Systemのシグネチャ(条件)
	unordered_map<const char*, Signature> m_signatures;

	// 型名 -> Systemのポインタ
	unordered_map<const char*, shared_ptr<System>> m_systems;
};