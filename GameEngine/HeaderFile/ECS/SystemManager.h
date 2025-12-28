////////////////////////////////
// 
// SystemManager.h
// 役割：各種System（ロジック）の登録と・Entityリストのメンテナンスを行う場所
// 作成開始日：2025/12/10
// 
////////////////////////////////

#pragma once

#include "ECSTypes.h"
#include <set>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>

// システム規定クラス
class System
{
public:
	std::set<Entity> m_entities;
};

// Systemマネージャークラス
class SystemManager
{
public:
	// システムを登録する
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		// ★修正済み：typename ではなく typeName を使用
		assert(m_systems.find(typeName) == m_systems.end() && "Registering system more than once.");

		// システムを作成して保存
		auto system = std::make_shared<T>();
		m_systems.insert({ typeName, system });
		return system;
	}

	// システムが担当する条件を設定する
	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		// ★修正済み：typename ではなく typeName を使用
		assert(m_systems.find(typeName) != m_systems.end() && "System used before registered.");

		m_signatures.insert({ typeName, signature });
	}

	// Entityが破棄されたとき
	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : m_systems)
		{
			auto const& system = pair.second;
			system->m_entities.erase(entity);
		}
	}

	// Entityの構成が変わった時
	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		for (auto const& pair : m_systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = m_signatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->m_entities.insert(entity);
			}
			else
			{
				system->m_entities.erase(entity);
			}
		}
	}

private:
	std::unordered_map<const char*, Signature> m_signatures;
	std::unordered_map<const char*, std::shared_ptr<System>> m_systems;
};