#pragma once

#include "SlotMap.h"
#include "ComponentType.h"

#include <array>

class Component;
class Entity;

#define INVALID_ENTITY INVALID_OBJECT
#define INVALID_COMPONENT INVALID_OBJECT

typedef obj_id entity_id;
typedef obj_id component_id;

class World {
public:
	static entity_id CreateEntity();
	static Entity* CreateAndGetEntity();
	
	static Entity* GetEntity(entity_id a_id);
	static bool IsEntityValid(entity_id a_id);
	
	static void DestroyEntity(entity_id a_id);

	static SlotMap<Entity>::iterator BeginEntities();
	static SlotMap<Entity>::iterator EndEntities();



	template <class T>
	static component_id CreateComponent();

	template <class T>
	static T* CreateAndGetComponent();

	template <class T>
	static T* GetComponent(component_id a_id);

	template <class T>
	static bool IsComponentValid(component_id a_id);

	template <class T>
	static void DestroyComponent(component_id a_id);
	static void DestroyComponent(component_index a_typeIndex, component_id a_id);

	template <class T>
	static typename SlotMap<T>::iterator BeginComponents();

	template <class T>
	static typename SlotMap<T>::iterator EndComponents();
private:
	template <class T>
	static SlotMap<T>* GetSlotMap();

	static SlotMap<Entity> s_entities;
	static std::array<SlotMapBase*, ComponentTypeIndex::COUNT> s_components;
};

template <class T>
component_id World::CreateComponent() {
	return GetSlotMap<T>()->CreateObject();
}

template <class T>
T* World::CreateAndGetComponent() {
	return GetComponent<T>(CreateComponent<T>());
}

template <class T>
T* World::GetComponent(const component_id a_id) {
	return static_cast<T*>(GetSlotMap<T>()->GetObject(a_id));
}

template <class T>
bool World::IsComponentValid(const component_id a_id) {
	return GetSlotMap<T>()->IsObjectValid(a_id);
}

template <class T>
void World::DestroyComponent(const component_id a_id) {
	DestroyComponent(ComponentTraits<T>::GetTypeIndex(), a_id);
}

template <class T>
typename SlotMap<T>::iterator World::BeginComponents() {
	return GetSlotMap<T>()->begin();
}

template <class T>
typename SlotMap<T>::iterator World::EndComponents() {
	return GetSlotMap<T>()->end();
}

template <class T>
SlotMap<T>* World::GetSlotMap() {
	const component_index typeIndex = ComponentTraits<T>::GetTypeIndex();
	SlotMap<T>* slotMap = static_cast<SlotMap<T>*>(s_components[typeIndex]);
	if (!slotMap) {
		slotMap = new SlotMap<T>();
		s_components[typeIndex] = slotMap;
	}
	return slotMap;
}
