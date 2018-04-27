#include "FpsController.h"
#include "Geometry.h"
#include "ContentManager.h"
#include "Input.h"
#include "Camera.h"
#include "SimState.h"

using namespace glm;
using namespace nlohmann;

FpsControllerDesc::FpsControllerDesc(): m_moveSpeed(0.25f), m_cameraSpeed(0.2f) {}

FpsControllerDesc::FpsControllerDesc(json& a_data) : FpsControllerDesc() {
	m_moveSpeed = ContentManager::FromJson(a_data, "MoveSpeed", m_moveSpeed);
	m_cameraSpeed = ContentManager::FromJson(a_data, "CameraSpeed", m_cameraSpeed);
}

void FpsControllerDesc::Create(Entity* a_entity) {
	auto* controller = World::CreateAndGetComponent<FpsController>();
	controller->Init(this);
	a_entity->AddComponent(controller);
}





FpsController::FpsController(): m_moveSpeed(0), m_cameraSpeed(0), m_locked(false) {}

component_type FpsController::GetType() {
	return Component::GetType() | (1 << GetTypeIndex());
}

component_index FpsController::GetTypeIndex() {
	return ComponentTypeIndex::FPS_CONTROLLER;
}

bool FpsController::Init(const FpsControllerDesc* a_desc) {
	m_moveSpeed = a_desc->m_moveSpeed;
	m_cameraSpeed = a_desc->m_cameraSpeed;

	return true;
}

void FpsController::On(const KeyboardEvent& a_event) {
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
	} else if (IS_KEY(a_event.m_key, ESCAPE) && a_event.m_began) {
		m_locked = !m_locked;
		return;
	} else {
		return;
	}

	if (m_locked) return;

	Transform& transform = GetEntity()->GetTransform();
	transform.Translate(m_moveSpeed * (a_event.m_shiftHeld ? 0.25f : 1.f) * direction);
}

void FpsController::On(const MouseMovedEvent& a_event) {
	if (!m_enabled || !m_active || m_locked) return;

	auto* camera = GetEntity()->GetComponent<Camera>();
	if (!camera || !camera->IsMode(CameraMode::FPS)) return;

	camera->TranslateAngles(m_cameraSpeed * SimState::Delta().GetSeconds() * a_event.m_fromCenter * -1.f);

	Input::Instance().ResetMouse();
}