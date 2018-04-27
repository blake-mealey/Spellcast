#include "PlayerController.h"
#include "Geometry.h"
#include "ContentManager.h"
#include "Input.h"
#include "Camera.h"
#include "SimState.h"

using namespace glm;
using namespace nlohmann;

PlayerControllerDesc::PlayerControllerDesc(): m_moveSpeed(0.5f), m_cameraSpeed(0.2f) {}

PlayerControllerDesc::PlayerControllerDesc(json& a_data) : PlayerControllerDesc() {
	m_moveSpeed = ContentManager::FromJson(a_data, "MoveSpeed", m_moveSpeed);
	m_cameraSpeed = ContentManager::FromJson(a_data, "CameraSpeed", m_cameraSpeed);
}

void PlayerControllerDesc::Create(Entity* a_entity) {
	auto* controller = World::CreateAndGetComponent<PlayerController>();
	controller->Init(this);
	a_entity->AddComponent(controller);
}





PlayerController::PlayerController(): m_moveSpeed(0), m_cameraSpeed(0) {}

component_type PlayerController::GetType() {
	return Component::GetType() | (1 << GetTypeIndex());
}

component_index PlayerController::GetTypeIndex() {
	return ComponentTypeIndex::PLAYER_CONTROLLER;
}

bool PlayerController::Init(const PlayerControllerDesc* a_desc) {
	m_moveSpeed = a_desc->m_moveSpeed;
	m_cameraSpeed = a_desc->m_cameraSpeed;

	return true;
}

void PlayerController::On(const KeyboardEvent& a_event) {
	if (!m_enabled || !m_active) return;
	if (a_event.m_ended) return;

	auto* camera = GetEntity()->GetComponent<Camera>();
	if (!camera || !camera->IsMode(CameraMode::FPS)) return;

	vec3 direction;
	if (IS_KEY(a_event.m_key, W)) {
		direction = camera->GetGlobalForward();
	} else if (IS_KEY(a_event.m_key, S)) {
		direction = -camera->GetGlobalForward();
	} else if (IS_KEY(a_event.m_key, D)) {
		direction = camera->GetGlobalRight();
	} else if (IS_KEY(a_event.m_key, A)) {
		direction = -camera->GetGlobalRight();
	} else if (IS_KEY(a_event.m_key, E)) {
		direction = camera->GetGlobalUp();
	} else if (IS_KEY(a_event.m_key, Q)) {
		direction = -camera->GetGlobalUp();
	} else {
		return;
	}

	Transform& transform = GetEntity()->GetTransform();
	transform.Translate(m_moveSpeed * direction);
}

void PlayerController::On(const MouseMovedEvent& a_event) {
	if (!m_enabled || !m_active) return;

	auto* camera = GetEntity()->GetComponent<Camera>();
	if (!camera || !camera->IsMode(CameraMode::FPS)) return;

	camera->TranslateAngles(m_cameraSpeed * SimState::Delta().GetSeconds() * a_event.m_fromCenter * -1.f);

	Input::Instance().ResetMouse();
}
