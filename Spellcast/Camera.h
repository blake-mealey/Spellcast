#pragma once

#include "Component.h"
#include "ContentManager.h"
#include "Geometry.h"
#include "MeshRenderer.h"

class Entity;

#define DEFAULT_NEAR_CLIPPING_PLANE 0.1f
#define DEFAULT_FAR_CLIPPING_PLANE 1000.f
#define DEFAULT_FIELD_OF_VIEW 60.f

class GraphicsSystem;

struct CameraDesc {
	CameraDesc() : m_nearClippingPlane(DEFAULT_NEAR_CLIPPING_PLANE), m_farClippingPlane(DEFAULT_FAR_CLIPPING_PLANE),
	               m_upVector(Geometry::UP), m_fieldOfView(glm::radians(DEFAULT_FIELD_OF_VIEW)), m_targetGlobalPosition(Geometry::FORWARD),
				   m_viewportUnitScale(glm::vec2(1.f)) {}

	explicit CameraDesc(nlohmann::json& a_data) : CameraDesc() {
		m_nearClippingPlane = ContentManager::FromJson(a_data, "NearClippingPlane", m_nearClippingPlane);
		m_farClippingPlane = ContentManager::FromJson(a_data, "FarClippingPlane", m_farClippingPlane);
		
		m_upVector = ContentManager::VecFromJson(a_data, "UpVector", m_upVector);
		m_fieldOfView = glm::radians(ContentManager::FromJson(a_data, "FieldOfView", DEFAULT_FIELD_OF_VIEW));

		m_globalPosition = ContentManager::VecFromJson(a_data, "Position", m_globalPosition);
		m_targetGlobalPosition = ContentManager::VecFromJson(a_data, "Target", m_targetGlobalPosition);

		m_viewportUnitScale = ContentManager::VecFromJson(a_data, "ViewportUnitScale", m_viewportUnitScale);
		m_viewportUnitPosition = ContentManager::VecFromJson(a_data, "ViewportUnitPosition", m_viewportUnitPosition);
		
		m_viewportPixelScale = ContentManager::VecFromJson(a_data, "ViewportPixelScale", m_viewportPixelScale);
		m_viewportPixelPosition = ContentManager::VecFromJson(a_data, "ViewportPixelPosition", m_viewportPixelPosition);
	}

	float m_nearClippingPlane;
	float m_farClippingPlane;
	
	glm::vec3 m_upVector;
	float m_fieldOfView;

	glm::vec3 m_globalPosition;
	glm::vec3 m_targetGlobalPosition;

	glm::vec2 m_viewportUnitScale;
	glm::vec2 m_viewportUnitPosition;
	
	glm::vec2 m_viewportPixelScale;
	glm::vec2 m_viewportPixelPosition;
};

class Camera : public Component {
public:
	Camera();
	static component_type GetType();
	static component_index GetTypeIndex();

	bool Init(const CameraDesc& a_desc);

	void Render(const GraphicsSystem& a_context);

private:
	Entity* m_entity;
	// MeshRenderer m_meshRenderer;

	float m_nearClippingPlane;
	float m_farClippingPlane;
	
	glm::vec3 m_upVector;
	float m_fieldOfView;

	glm::vec3 m_globalPosition;
	glm::vec3 m_targetGlobalPosition;

	glm::vec2 m_viewportUnitScale;
	glm::vec2 m_viewportUnitPosition;
	
	glm::vec2 m_viewportPixelScale;
	glm::vec2 m_viewportPixelPosition;
};
