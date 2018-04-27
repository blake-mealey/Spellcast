#pragma once

#include "Component.h"
#include "World.h"
#include "Transform.h"

#include <json/json.hpp>

#include <vector>
#include <array>
#include <unordered_set>

struct EntityDesc {
	explicit EntityDesc(nlohmann::json& a_data);
	Entity* Create();

	Transform m_transform;
	std::vector<ComponentDesc*> m_componentDescs;
	std::vector<EntityDesc*> m_childrenDescs;
};

class Entity {
friend SlotMap<Entity>;
friend EntityDesc;
public:
	~Entity();
	
	entity_id GetId() const;
	
	Transform& GetTransform();
	
	void SetParent(entity_id a_parent);

	// NOTE: Should _only_ be called from ComponentDesc::Create(Entity*) implementations
	template <class T>
	void AddComponent(T* a_component);

	template <class T>
	T* GetComponent() const;

	template <class T>
	std::vector<T*> GetComponents() const;
private:
	Entity();
	
	bool m_active;
	entity_id m_id;

	Transform m_transform;

	entity_id m_parent;
	std::unordered_set<entity_id> m_children;

	std::array<std::vector<component_id>, ComponentTypeIndex::COUNT> m_components;
};

template <class T>
void Entity::AddComponent(T* a_component) {
	a_component->m_entity = m_id;
	m_components[ComponentTraits<T>::GetTypeIndex()].push_back(a_component->GetId());
}

template <class T>
T* Entity::GetComponent() const {
	const std::vector<component_id>& list = m_components[ComponentTraits<T>::GetTypeIndex()];
	if (list.empty()) return nullptr;
	return World::GetComponent<T>(list[0]);
}

template <class T>
std::vector<T*> Entity::GetComponents() const {
	const std::vector<component_id>& ids = m_components[ComponentTraits<T>::GetTypeIndex()];
	std::vector<T*> components;
	for (component_id id : ids) components.push_back(World::GetComponent<T>(id));
	return components;
}
