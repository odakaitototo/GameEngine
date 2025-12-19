///////////////////////////////////////////////////////////////////////////////////////////////////
// 
// ComponentManager.h
// 
// 役割：全種類のコンポーネント配列を管理する場所
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
#include <array>
#include <unordered_map>
#include <memory>
#include <cassert>

// 基底クラス
// テンプレートクラスをリスト化するために、共通の親クラスを作る
class IComponentArray
{
public:

	// コンポーネントをEntityに追加
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0; // 「Entityが死んだ時の処理」だけは共通して持っておいてという約束

};



// データ管理クラス（Packed Array）
// ここに実際のデータが「ぎっちり」詰まって入ります
template<typename T>
class ComponentArray : public IComponentArray // スーパーセットの実体
{
public:

	// コンポーネントをEntityに追加
	void InsertData(Entity entity, T component)
	{
		assert(m_entityToIndexMap.find(entity) == m_entityToIndexMap.end() && "Component added to same entity more than once."); // 既に持っているのに追加しようとしていないかチェック
		 // 新しいデータを配列の末尾に追加
		size_t newIndex = m_size;
		m_entityToIndexMap[entity] = newIndex; // 地図(Map)に記録：「このEntityのデータは、配列のnewIndex番目にあるよ」
		m_indexToEntityMap[newIndex] = entity; // 逆引き辞書も記録：「このEntityのデータは、配列のnewIndex番目にあるよ」
		m_componentArray[newIndex] = component; // 実際のデータを配列に保存
		m_size++;
	}

	// コンポーネントを消去（重要：配列の穴埋め処理）
	void RemoveData(Entity entity)
	{
		assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Removing non-existent component.");
		
		// 消去するデータの位置
		size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
		size_t indexOfLastElement = m_size - 1;

		// 配列の一番後ろにあるデータを、消去して空いた穴に移動させる（上書きコピー）
		m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastElement];

		Entity entityOfLastElement = m_indexToEntityMap[indexOfLastElement]; // 移動してきたデータの持ち主(Entity)を特定

		// マップ情報も更新(「末尾にいたあの子、消去された穴の場所に移動した」)
		m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		// 古いキーを消去
		m_entityToIndexMap.erase(entity);
		m_indexToEntityMap.erase(indexOfLastElement);

		m_size--;

	}

private:
	
	array<T, MAX_ENTITIES> m_componentArray; // 実際のコンポーネントデータ（ここがメモリ上で連続になる！）メモリ上で隙間なく並ぶので超高速

	unordered_map<Entity, size_t> m_entityToIndexMap; // Entity ID -> 配列インデックスの変換マップ。スパース配列（地図）

	unordered_map<size_t, Entity> m_indexToEntityMap; // 配列インデックス -> Entity ID の逆引きマップ

	size_t m_size = 0; // 現在のデータ数
};


// マネージャー
// 各コンポーネントごとのComponentArrayを管理します
class ComponentManager
{
public:

	// コンポーネントの「型」登録（ゲーム機同時に呼ぶ）
	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();
		assert(m_componentTypes.find(typeName) == m_componentTypes.end() && "Registering component type more than once");

		m_componentTypes[typeName] = m_nextComponentType; // コンポーネントの種類にIDを割り振る

		m_componentArrays.insert({ typeName, make_shared<ComponentArray<T>>() }); // データを格納する配列を作成

		m_nextComponentType++;
	}

	// コンポーネントの種類IDを取得
	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();
		assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component not registered before use.");
		return m_componentTypes[typeName];

	}

	// Entityからコンポ―ネントを削除
	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}

	// コンポーネントを取得
	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	// Entityが破棄されたとき、持っている全コンポーネントを消去通知
	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : m_componentArrays)
		{
			auto const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}

private:

	// 内部ヘルパー：型Tに対応するComponentArrayを取得
	template<typename T>
	shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();
		assert(m_componentTypes.find(typeName) != m_componentType.end() && "Component not registered befor use.");
		return static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
	}

	unordered_map<const char*, ComponentType> m_componentTypes; // 型名文字列 -> コンポーネント種類IDのマップ

	unordered_map < const char*, shared_ptr<IComponentArray>> m_componentArrays; // 型名文字列 -> コンポーネント配列クラスのマップ

	ComponentType m_nextComponentType{}; // 次に割り振るコンポーネント種類ID


};