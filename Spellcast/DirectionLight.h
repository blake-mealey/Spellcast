#pragma once

#include "Component.h"
#include "ShadowCaster.h"

#include <json/json.hpp>
#include <glm/glm.hpp>

class DirectionLight;

struct DirectionLightDesc : ComponentDesc {
	DirectionLightDesc();
	explicit DirectionLightDesc(nlohmann::json& a_data);
	void Create(Entity* a_entity) override;

	glm::vec3 m_color;
	glm::vec3 m_direction;

	float m_shadowIntensity;
	bool m_castsShadows;
	int m_shadowMapSize;
};

class DirectionLight : public Component, public ShadowCaster {
	friend SlotMap<DirectionLight>;
public:
	~DirectionLight() override;
	DirectionLight();
	static component_type GetType();
	static component_index GetTypeIndex();
	
	bool Init(const DirectionLightDesc& a_desc);
	bool InitShadowMap() override;
	
	void RenderShadowMap() override;
	void RenderShadows(const glm::mat4& a_viewProjectionMatrix) const override;

	void ComputeDepthViewProjectionMatrix() override;

	const glm::vec3& GetColor() const;
	const glm::vec3& GetDirection() const;
	void SetDirection(const glm::vec3& a_direction);
private:
	glm::vec3 m_color;
	glm::vec3 m_direction;
};
