#pragma once

#include "Component.h"

#include <json/json.hpp>
#include <glm/glm.hpp>

class Entity;

#define DEFAULT_NEAR_CLIPPING_PLANE 0.1f
#define DEFAULT_FAR_CLIPPING_PLANE 1000.f
#define DEFAULT_FIELD_OF_VIEW 60.f

class GraphicsSystem;

class Camera;

struct CameraDesc : ComponentDesc {
	CameraDesc();
	explicit CameraDesc(nlohmann::json& a_data);
	void Create(Entity* a_entity) override;

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
friend SlotMap<Camera>;
public:
	~Camera() override = default;
	Camera();
	static component_type GetType();
	static component_index GetTypeIndex();

	bool Init(const CameraDesc& a_desc);

	void Render(const GraphicsSystem& a_context) const;

private:
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
