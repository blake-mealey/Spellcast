#include "World.h"
#include "Entity.h"

using namespace std;

SlotMap<Entity> World::s_entities;
array<SlotMapBase*, ComponentTypeIndex::COUNT> World::s_components;

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

SlotMap<Entity>::iterator World::BeginEntities() {
	return s_entities.begin();
}

SlotMap<Entity>::iterator World::EndEntities() {
	return s_entities.end();
}

void World::DestroyComponent(const component_index a_typeIndex, const component_id a_id) {
	static_cast<SlotMap<Component>*>(s_components[a_typeIndex])->DestroyObject(a_id);  // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
}
