#include "Entity.h"

using namespace std;

Entity::~Entity() {
	for (vector<Component*>& components : m_components) {
		for (Component* component : components) {
			delete component;
		}
		components.clear();
	}
	m_active = false;
	m_parent = NO_PARENT;
}

Entity::Entity() : m_active(false), m_id(-1), m_parent(NO_PARENT), m_components({}) {}
