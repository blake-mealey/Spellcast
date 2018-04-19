#include "EntityManager.h"
#include "Entity.h"

#define CHUNK_SIZE 3

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
	Entity& entity = s_entityTable[free / CHUNK_SIZE][free % CHUNK_SIZE];
	entity.m_active = true;
	return entity.m_id;
}

Entity* EntityManager::CreateAndGetEntity() {
	return GetEntity(CreateEntity());
}

Entity* EntityManager::GetEntity(const entity_id a_id) {
	const int lowOrder = a_id & 0xFFFFFFFF;
	Entity* entity = s_entityTable[lowOrder / CHUNK_SIZE] + (lowOrder % CHUNK_SIZE);
	return entity->m_id != a_id ? nullptr : entity;
}

bool EntityManager::IsEntityValid(const entity_id a_id) {
	return GetEntity(a_id) != nullptr;
}

void EntityManager::DestroyEntity(const entity_id a_id) {
	Entity* entity = GetEntity(a_id);
	if (!entity) return;
	entity->m_id = (a_id & 0xFFFFFFFF) | (((a_id >> 32) + 1) << 32);
	entity->~Entity();
	s_freeList.push_back(a_id & 0xFFFFFFFF);
}

void EntityManager::iterator::iterate() {
	do {
		m_ptr++;
	} while (!m_ptr->m_active && m_ptr != m_end);
}

EntityManager::iterator EntityManager::begin() {
	return iterator(s_entityTable[0], end_ptr());
}

EntityManager::iterator EntityManager::end() {
	return iterator(end_ptr(), end_ptr());
}

Entity* EntityManager::end_ptr() {
	return s_entityTable[0] + s_entityTable.size()*CHUNK_SIZE;
}
