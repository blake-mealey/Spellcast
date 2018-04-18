#pragma once

#include "Component.h"
#include "EntityManager.h"

#include <vector>
#include <array>

class Entity {
friend EntityManager;
public:
	~Entity();
	
	entity_id m_id;

	template <class T>
	void AddComponent(T* a_component);

	template <class T>
	T* GetComponent();

	template <class T>
	const std::vector<T*>& GetComponents();

private:
	Entity() = default;

	std::array<std::vector<Component*>, ComponentTypeIndex::COUNT> m_components;
};

template <class T>
void Entity::AddComponent(T* a_component) {
	m_components[ComponentTraits<T>::GetTypeIndex()].push_back(a_component);
}

template <class T>
T* Entity::GetComponent() {
	std::vector<Component*>& list = m_components[ComponentTraits<T>::GetTypeIndex()];
	if (list.empty()) return nullptr;
	return static_cast<T*>(list[0]);
}

template <class T>
const std::vector<T*>& Entity::GetComponents() {
	return m_components[ComponentTraits<T>::GetTypeIndex()];
}
