#include "EntityManager.h"
#include "Entity.h"

#define CHUNK_SIZE 256

using namespace std;

vector<Entity*> EntityManager::s_entityTable;
vector<int> EntityManager::s_freeList;

entity_id EntityManager::CreateEntity() {
	if (s_freeList.empty()) {
		auto* chunk = new Entity[CHUNK_SIZE];
		for (int i = CHUNK_SIZE - 1; i >= 0; --i) {
			chunk[i].m_id = s_entityTable.size() * CHUNK_SIZE + i;
			s_freeList.push_back(s_entityTable.size() * CHUNK_SIZE + i);
		}
		s_entityTable.push_back(chunk);
	}

	const int free = s_freeList.back();
	s_freeList.pop_back();
	return s_entityTable[free / CHUNK_SIZE][free % CHUNK_SIZE].m_id;
}

Entity* EntityManager::CreateAndGetEntity() {
	return GetEntity(CreateEntity());
}

Entity* EntityManager::GetEntity(entity_id a_id) {
	const int lowOrder = a_id & 0xFFFFFFFF;
	Entity* entity = s_entityTable[lowOrder / CHUNK_SIZE] + (lowOrder % CHUNK_SIZE);
	return entity->m_id != a_id ? nullptr : entity;
}

void EntityManager::DestroyEntity(entity_id a_id) {
	DestroyEntity(GetEntity(a_id));
}

void EntityManager::DestroyEntity(Entity* a_entity) {
	if (!a_entity) return;
	a_entity->m_id = (a_entity->m_id & 0xFFFFFFFF) | (((a_entity->m_id >> 32) + 1) << 32);
	a_entity->~Entity();
	s_freeList.push_back(a_entity->m_id & 0xFFFFFFFF);
}
