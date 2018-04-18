#pragma once

#include <vector>

class Entity;

typedef long long entity_id;

class EntityManager {
public:
	static entity_id CreateEntity();
	static Entity* CreateAndGetEntity();
	
	static Entity* GetEntity(entity_id a_id);
	
	static void DestroyEntity(entity_id a_id);
	static void DestroyEntity(Entity* a_entity);

private:
	static std::vector<Entity*> s_entityTable;
	static std::vector<int> s_freeList;
};
