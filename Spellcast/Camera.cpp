#include "Camera.h"
#include "GraphicsSystem.h"

#include <glm/gtc/matrix_transform.inl>

using namespace glm;

Camera::Camera() : m_nearClippingPlane(0.f), m_farClippingPlane(0.f), m_fieldOfView(0.f) {};

component_type Camera::GetType() const {
	return Component::GetType() | ComponentType::CAMERA;
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

	m_meshRenderer.Init(MeshRendererDesc(ContentManager::GetJsonData(ContentManager::GetContentPath("Boulder.comp.json"))));

	return true;
}

void Camera::Render(const GraphicsSystem& a_context) {
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
	
	// TODO: Render everything
	m_meshRenderer.GetTransform().Rotate(Geometry::UP, 0.01f);
	m_meshRenderer.Render(viewMatrix, projectionMatrix);
}
