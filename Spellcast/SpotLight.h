#pragma once

#include "Component.h"
#include "Transform.h"
#include "ShadowCaster.h"

#include <glm/glm.hpp>

class SpotLightDesc : public ComponentDesc {
public:
	SpotLightDesc();
	explicit SpotLightDesc(nlohmann::json& a_data);
	void Create(Entity* a_entity) override;

	glm::vec3 m_color;
	float m_power;

	Transform m_transform;
	glm::vec3 m_direction;
	float m_angleDegrees;

	float m_shadowIntensity;
	bool m_castsShadows;
	int m_shadowMapSize;
};

class SpotLight : public Component, ShadowCaster {
friend SlotMap<SpotLight>;
public:
	~SpotLight() override = default;
	SpotLight();
	static component_type GetType();
	static component_index GetTypeIndex();

	bool Init(const SpotLightDesc& a_desc);
	
	void RenderShadowMap() override;
	void RenderShadows(const glm::mat4& a_viewProjectionMatrix) const;

	void ComputeDepthViewProjectionMatrix() override;

	const glm::vec3& GetColor() const;
	float GetPower() const;

	const Transform& GetTransform() const;
	Transform& GetTransform();
	glm::vec3 GetGlobalPosition() const;

	const glm::vec3& GetDirection() const;
	glm::vec3 GetGlobalDirection() const;
	void SetDirection(glm::vec3 a_direction);

	float GetAngleDegrees() const;
	float GetAngleRadians() const;
private:
	glm::vec3 m_color;
	float m_power;

	Transform m_transform;
	glm::vec3 m_direction;
	float m_angleDegrees;
};
