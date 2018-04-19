#include "Camera.h"
#include "GraphicsSystem.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Geometry.h"

#include <glm/gtc/matrix_transform.inl>

using namespace glm;
using namespace nlohmann;

CameraDesc::CameraDesc(): m_nearClippingPlane(DEFAULT_NEAR_CLIPPING_PLANE),
                          m_farClippingPlane(DEFAULT_FAR_CLIPPING_PLANE),
                          m_upVector(Geometry::UP), m_fieldOfView(radians(DEFAULT_FIELD_OF_VIEW)),
                          m_targetGlobalPosition(Geometry::FORWARD),
                          m_viewportUnitScale(vec2(1.f)) {}

CameraDesc::CameraDesc(json& a_data): CameraDesc() {
	m_nearClippingPlane = ContentManager::FromJson(a_data, "NearClippingPlane", m_nearClippingPlane);
	m_farClippingPlane = ContentManager::FromJson(a_data, "FarClippingPlane", m_farClippingPlane);

	m_upVector = ContentManager::VecFromJson(a_data, "UpVector", m_upVector);
	m_fieldOfView = radians(ContentManager::FromJson(a_data, "FieldOfView", DEFAULT_FIELD_OF_VIEW));

	m_globalPosition = ContentManager::VecFromJson(a_data, "Position", m_globalPosition);
	m_targetGlobalPosition = ContentManager::VecFromJson(a_data, "Target", m_targetGlobalPosition);

	m_viewportUnitScale = ContentManager::VecFromJson(a_data, "ViewportUnitScale", m_viewportUnitScale);
	m_viewportUnitPosition = ContentManager::VecFromJson(a_data, "ViewportUnitPosition", m_viewportUnitPosition);

	m_viewportPixelScale = ContentManager::VecFromJson(a_data, "ViewportPixelScale", m_viewportPixelScale);
	m_viewportPixelPosition = ContentManager::VecFromJson(a_data, "ViewportPixelPosition", m_viewportPixelPosition);
}

void CameraDesc::Create(Entity* a_entity) {
	auto* camera = new Camera();
	camera->Init(*this);
	a_entity->AddComponent(camera);
}




Camera::Camera() : m_nearClippingPlane(0.f), m_farClippingPlane(0.f), m_fieldOfView(0.f) {};

component_type Camera::GetType() {
	return Component::GetType() | ComponentType::CAMERA;
}

component_index Camera::GetTypeIndex() {
	return ComponentTypeIndex::CAMERA;
}

bool Camera::Init(const CameraDesc& a_desc) {
	m_nearClippingPlane = a_desc.m_nearClippingPlane;
	m_farClippingPlane = a_desc.m_farClippingPlane;
	
	m_upVector = a_desc.m_upVector;
	m_fieldOfView = a_desc.m_fieldOfView;

	m_globalPosition = a_desc.m_globalPosition;
	m_targetGlobalPosition = a_desc.m_targetGlobalPosition;

	m_viewportUnitScale = a_desc.m_viewportUnitScale;
	m_viewportUnitPosition = a_desc.m_viewportUnitPosition;
	
	m_viewportPixelScale = a_desc.m_viewportPixelScale;
	m_viewportPixelPosition = a_desc.m_viewportPixelPosition;

	return true;
}

void Camera::Render(const GraphicsSystem& a_context) const {
	// Compute current viewport
	const vec2& windowDims = a_context.GetWindowDims();
	const vec2 viewportPosition = (windowDims * m_viewportUnitPosition) + m_viewportPixelPosition;
	const vec2 viewportScale = (windowDims * m_viewportUnitScale) + m_viewportPixelScale;
	glViewport(viewportPosition.x, viewportPosition.y, viewportScale.x, viewportScale.y);

	// Compute current aspect ratio
	const float aspectRatio = viewportScale.x / viewportScale.y;

	// Compute view and projection matrices
	const mat4 viewMatrix = lookAt(m_globalPosition, m_targetGlobalPosition, m_upVector);
	const mat4 projectionMatrix = perspective(m_fieldOfView, aspectRatio, m_nearClippingPlane, m_farClippingPlane);

	// Render meshes
	for (auto it = EntityManager::begin(); it != EntityManager::end(); ++it) {
		Entity& entity = *it;
		for (MeshRenderer* meshRenderer : entity.GetComponents<MeshRenderer>()) {
			meshRenderer->Render(viewMatrix, projectionMatrix);
		}
		entity.GetTransform().Rotate(Geometry::UP, 0.01f);
	}
}
