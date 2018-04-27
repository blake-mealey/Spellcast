#include "PlayerController.h"
#include "Geometry.h"
#include "ContentManager.h"

using namespace glm;
using namespace nlohmann;

PlayerControllerDesc::PlayerControllerDesc(): m_speed(0) {}

PlayerControllerDesc::PlayerControllerDesc(json& a_data) {
	m_speed = ContentManager::FromJson(a_data, "Speed", 1.f);
}

void PlayerControllerDesc::Create(Entity* a_entity) {
	auto* controller = World::CreateAndGetComponent<PlayerController>();
	controller->Init(this);
	a_entity->AddComponent(controller);
}





PlayerController::PlayerController(): m_speed(0) {}

component_type PlayerController::GetType() {
	return Component::GetType() | (1 << GetTypeIndex());
}

component_index PlayerController::GetTypeIndex() {
	return ComponentTypeIndex::PLAYER_CONTROLLER;
}

bool PlayerController::Init(const PlayerControllerDesc* a_desc) {
	m_speed = a_desc->m_speed;

	return true;
}

void PlayerController::On(const KeyboardEvent& a_event) {
	if (!m_enabled || !m_active) return;
	if (a_event.m_released) return;

	vec3 direction;
	if (IS_KEY(a_event.m_key, W)) {
		direction = Geometry::FORWARD;
	} else if (IS_KEY(a_event.m_key, S)) {
		direction = -Geometry::FORWARD;
	} else if (IS_KEY(a_event.m_key, D)) {
		direction = Geometry::RIGHT;
	} else if (IS_KEY(a_event.m_key, A)) {
		direction = -Geometry::RIGHT;
	} else if (IS_KEY(a_event.m_key, E)) {
		direction = Geometry::UP;
	} else if (IS_KEY(a_event.m_key, Q)) {
		direction = -Geometry::UP;
	}

	if (length(direction) == 0.f) return;
	Transform& transform = GetEntity()->GetTransform();
	transform.Translate(normalize(direction) * m_speed);
}

void PlayerController::On(const MouseMovedEvent& a_event) {
	
}





