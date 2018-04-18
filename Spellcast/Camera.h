#pragma once

#include "Component.h"
#include "ContentManager.h"
#include "Geometry.h"

class GraphicsSystem;

struct CameraDesc {
	explicit CameraDesc(nlohmann::json& a_data) {
		m_globalPosition = ContentManager::VecFromJson(a_data, "Position", glm::vec3(0.f));
		m_targetGlobalPosition = ContentManager::VecFromJson(a_data, "Target", Geometry::FORWARD);

		m_viewportUnitScale = ContentManager::VecFromJson(a_data, "ViewportUnitScale", glm::vec2(1.f));
		m_viewportUnitPosition = ContentManager::VecFromJson(a_data, "ViewportUnitPosition", glm::vec2(0.f));
		
		m_viewportPixelScale = ContentManager::VecFromJson(a_data, "ViewportPixelScale", glm::vec2(0.f));
		m_viewportPixelPosition = ContentManager::VecFromJson(a_data, "ViewportPixelPosition", glm::vec2(0.f));
	}

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
	component_type GetType() const override;

	bool Init(const CameraDesc& a_desc);

	void Render(const GraphicsSystem& a_context) const;

private:
	glm::vec3 m_globalPosition;
	glm::vec3 m_targetGlobalPosition;

	glm::vec2 m_viewportUnitScale;
	glm::vec2 m_viewportUnitPosition;
	
	glm::vec2 m_viewportPixelScale;
	glm::vec2 m_viewportPixelPosition;
};
