#include "EntityManager.h"
#include "Entity.h"

#define CHUNK_SIZE 3

using namespace std;

SlotMap<Entity> EntityManager::s_entities;

entity_id EntityManager::CreateEntity() {
	return s_entities.CreateObject();
}

Entity* EntityManager::CreateAndGetEntity() {
	return s_entities.CreateAndGetObject();
}

Entity* EntityManager::GetEntity(const entity_id a_id) {
	return s_entities.GetObject(a_id);
}

bool EntityManager::IsEntityValid(const entity_id a_id) {
	return s_entities.IsObjectValid(a_id);
}

void EntityManager::DestroyEntity(const entity_id a_id) {
	s_entities.DestroyObject(a_id);
}

SlotMap<Entity>::iterator EntityManager::begin() {
	return s_entities.begin();
}

SlotMap<Entity>::iterator EntityManager::end() {
	return s_entities.end();
}
