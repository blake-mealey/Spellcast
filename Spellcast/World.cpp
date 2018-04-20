#include "World.h"
#include "Entity.h"

#define CHUNK_SIZE 3

using namespace std;

SlotMap<Entity> World::s_entities;

entity_id World::CreateEntity() {
	return s_entities.CreateObject();
}

Entity* World::CreateAndGetEntity() {
	return s_entities.CreateAndGetObject();
}

Entity* World::GetEntity(const entity_id a_id) {
	return s_entities.GetObject(a_id);
}

bool World::IsEntityValid(const entity_id a_id) {
	return s_entities.IsObjectValid(a_id);
}

void World::DestroyEntity(const entity_id a_id) {
	s_entities.DestroyObject(a_id);
}

SlotMap<Entity>::iterator World::begin() {
	return s_entities.begin();
}

SlotMap<Entity>::iterator World::end() {
	return s_entities.end();
}
