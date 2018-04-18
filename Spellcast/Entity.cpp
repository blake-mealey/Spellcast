#include "Entity.h"

using namespace std;

Entity::~Entity() {
	for (vector<Component*>& components : m_components) {
		for (Component* component : components) {
			delete component;
		}
		components.clear();
	}
}
