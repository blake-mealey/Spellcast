#include "Entity.h"
#include <iostream>
#include "ContentManager.h"

using namespace std;
using namespace nlohmann;

EntityDesc::EntityDesc(json& a_data) {
	for (json& child : a_data["Children"]) {
		m_childrenDescs.push_back(ContentManager::GetEntityDesc(child));
	}

	for (json& component : a_data["Components"]) {
		m_componentDescs.push_back(ContentManager::GetComponentDesc(component));
	}
}

entity_id EntityDesc::Create() {
	Entity* entity = EntityManager::CreateAndGetEntity();

	for (EntityDesc* desc : m_childrenDescs) {
		const entity_id child = desc->Create();
		EntityManager::GetEntity(child)->SetParent(entity->GetId());
	}

	for (ComponentDesc* desc : m_componentDescs) {
		desc->Create(entity);
	}

	return entity->GetId();
}



Entity::Entity() : m_active(false), m_id(INVALID_ENTITY), m_parent(INVALID_ENTITY), m_components({}) {}

Entity::~Entity() {
	m_active = false;
	
	m_parent = INVALID_ENTITY;
	for (entity_id id : m_children) {
		EntityManager::DestroyEntity(id);
	}

	for (vector<Component*>& components : m_components) {
		for (Component* component : components) {
			delete component;
		}
		components.clear();
	}
}

entity_id Entity::GetId() const {
	return m_id;
}

void Entity::SetParent(entity_id a_parent) {
	// Check that the new parent is not the same as the old parent
	if (m_parent == a_parent) return;

	// Check that the new parent exists
	Entity* newParent = EntityManager::GetEntity(a_parent);
	if (!newParent) {
		cerr << "WARNING: Attempt to set parent to nullptr." << endl;
		return;
	}

	// If we have a parent, remove ourselves from it
	if (m_parent != INVALID_ENTITY) {
		Entity* oldParent = EntityManager::GetEntity(m_parent);
		oldParent->m_children.erase(m_id);
	}

	// Add ourselves to the new parent, and update our parent's id
	newParent->m_children.insert(m_id);
	m_parent = a_parent;
}
