#pragma once

#include "Component.h"
#include "EntityManager.h"

#include <vector>
#include <array>

class Entity {
friend EntityManager;
public:
	~Entity();

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

	entity_id m_parent;
	std::vector<entity_id> m_children;

	std::array<std::vector<Component*>, ComponentTypeIndex::COUNT> m_components;
};

template <class T>
void Entity::AddComponent(T* a_component) {
	m_components[ComponentTraits<T>::GetTypeIndex()].push_back(a_component);
}

template <class T>
T* Entity::GetComponent() const {
	const std::vector<Component*>& list = m_components[ComponentTraits<T>::GetTypeIndex()];
	if (list.empty()) return nullptr;
	return static_cast<T*>(list[0]);
}

template <class T>
std::vector<T*> Entity::GetComponents() const {
	const std::vector<Component*>& components = m_components[ComponentTraits<T>::GetTypeIndex()];
	std::vector<T*> casted;

	if (components.empty()) return casted;

	casted.resize(components.size());
	for (size_t i = 0; i < components.size(); ++i) casted[i] = static_cast<T*>(components[i]);

	return casted;
}
