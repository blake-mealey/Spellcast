#pragma once

#include "SlotMap.h"
#include "ComponentType.h"

class Component;
class Entity;

#define INVALID_ENTITY -1

typedef obj_id entity_id;
typedef obj_id component_id;

class World {
public:
	static entity_id CreateEntity();
	static Entity* CreateAndGetEntity();
	
	static Entity* GetEntity(entity_id a_id);
	static bool IsEntityValid(entity_id a_id);
	
	static void DestroyEntity(entity_id a_id);

	static SlotMap<Entity>::iterator begin();
	static SlotMap<Entity>::iterator end();

private:
	static SlotMap<Entity> s_entities;
	static std::array<SlotMap<Component>, ComponentTypeIndex::COUNT> s_components;
};
