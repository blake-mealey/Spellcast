#include "Camera.h"
#include "GraphicsSystem.h"

using namespace glm;

Camera::Camera() = default;

component_type Camera::GetType() const {
	return Component::GetType() | ComponentType::CAMERA;
}

bool Camera::Init(const CameraDesc& a_desc) {
	m_globalPosition = a_desc.m_globalPosition;
	m_targetGlobalPosition = a_desc.m_targetGlobalPosition;

	m_viewportUnitScale = a_desc.m_viewportUnitScale;
	m_viewportUnitPosition = a_desc.m_viewportUnitPosition;
	
	m_viewportPixelScale = a_desc.m_viewportPixelScale;
	m_viewportPixelPosition = a_desc.m_viewportPixelPosition;

	return true;
}

void Camera::Render(const GraphicsSystem& a_context) const {
	const vec2& windowDims = a_context.GetWindowDims();
	const vec2 viewportPosition = (windowDims * m_viewportUnitPosition) + m_viewportPixelPosition;
	const vec2 viewportScale = (windowDims * m_viewportUnitScale) + m_viewportPixelScale;
	glViewport(viewportPosition.x, viewportPosition.y, viewportScale.x, viewportScale.y);
}
