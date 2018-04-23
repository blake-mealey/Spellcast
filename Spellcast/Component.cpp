#include "Component.h"

Component::Component() : m_enabled(true), m_id(INVALID_COMPONENT), m_active(false) {}

Component::~Component() {
	m_active = false;
	m_enabled = true;
}

component_type Component::GetId() const {
	return m_id;
}

void Component::SetEnabled(bool a_enabled) {
	m_enabled = a_enabled;
}

bool Component::IsEnabled() const {
	return m_enabled;
}

component_type Component::GetType() {
	return ComponentType::BASE_COMPONENT;
}

component_index Component::GetTypeIndex() {
	return ComponentTypeIndex::BASE_COMPONENT;
}
