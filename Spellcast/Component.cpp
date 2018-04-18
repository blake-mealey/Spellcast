#include "Component.h"

Component::Component() : m_enabled(true) {}
Component::~Component() = default;

void Component::SetEnabled(bool a_enabled) {
	m_enabled = a_enabled;
}

bool Component::IsEnabled() const {
	return m_enabled;
}

component_type Component::GetType() const {
	return ComponentType::BASE_COMPONENT;
}
