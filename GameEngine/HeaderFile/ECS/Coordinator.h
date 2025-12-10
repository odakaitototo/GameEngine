////////////////////////////////
// 
// Coordinator.h
// 役割：ECS全体のファザード（窓口）クラス
// 作成開始日：2025/12/10
// 
// 
// 
// 
// 
////////////////////////////////

////////////////////----- 単語帳 -----////////////////////////////
//
// ・。
// 
// 
///////////////////////////////////////////////////////////////////

#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "SystemManager.h"
#include <memory>

class Coordinator
{
public:
	void Init()
	{
		// 3大マネージャーを生成
		m_entityManager = make_unique<EntityManager>();
		m_componentManager = make_unique<ComponentManager>();
		m_systemManager = make_unique<SystemManager>();

	}

	// --- Entity管理 ---

	Entity CreateEntity()
	{
		return m_entityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		m_entityManager->DestroyEntity(entity);
		m_componentManager->EntityDestroyed(entity);
		m_systemManager->EntityDestroyed(entity);
	}

	// --- Component管理 ---

	template<typename T >
	void RegisterComponent()
	{
		m_componentManager->RegisterComponent<T>();
	}

	template<typename T >
	void AddComponent(Entity entity, T component)
	{
		m_componentManager->AddComponent<T>(entity, component);

		// Componentを追加したので、EntityのSignature（構成）を更新する
		auto signature = m_entityManager->GetSignature(entity);
		signature.set(m_componentManager->GetComponentType<T>(), true);
		m_entityManager->GetSignature(entity, signature);

		// System側に「構成が変わったからチェックしてね」と通知
		m_systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename>
	void RemoveComponent(Entity entity)
	{
		m_componentManager->RemoveComponent<T>(entity);
		// Componentを削除したので、Signatureを更新
		auto signature = m_entityManager->GetSignature(entity);
		signature.set(m_componentManager->GetComponentType<T>(), false);
		m_entityManager->SetSignature(entity, signature);

		m_systemManager->EntitySignatureChanged(entity.signature);
	}

	template<typename T >
	T& GetComponent(Entity entity)
	{
		return m_componentManager->GetComponent<T>();
	}


	template<typename T >
	ComponentType GetComponentType()
	{
		return m_componentManager->GetComponentType<T>();
	}

	// --- Systemの管理 ---

	template<typename T >
	shared_ptr<T> RegisterSystem()
	{
		rturn m_systemManager->RegisterSystem<T>();
	}

	template<typename T >
	void SetsystemSignature(Signature signature)
	{
		m_systemManager->SetSignature<T>(signature);
	}
private:

	unique_ptr<EntityManager> m_entityManager;
	unique_ptr<ComponentManager> m_componentManager;
	unique_ptr<SystemManager> m_systemManager;

};
