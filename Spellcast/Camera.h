#pragma once

#include "Component.h"
#include "ContentManager.h"
#include "MeshRenderer.h"

class Entity;

#define DEFAULT_NEAR_CLIPPING_PLANE 0.1f
#define DEFAULT_FAR_CLIPPING_PLANE 1000.f
#define DEFAULT_FIELD_OF_VIEW 60.f

class GraphicsSystem;

struct CameraDesc : ComponentDesc {
	CameraDesc();
	explicit CameraDesc(nlohmann::json& a_data);
	Component* Create() override;

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

	void Render(const GraphicsSystem& a_context) const;

private:
	Entity* m_entity;

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
