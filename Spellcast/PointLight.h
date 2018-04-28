#pragma once

#include "Component.h"
#include "Transform.h"

#include <glm/glm.hpp>
#include <json/json.hpp>

class PointLightDesc : public ComponentDesc {
public:
	PointLightDesc();
	explicit PointLightDesc(nlohmann::json& a_data);
	void Create(Entity* a_entity) override;

	glm::vec3 m_color;
	float m_power;

	Transform m_transform;
};

class PointLight : public Component {
friend SlotMap<PointLight>;
public:
	~PointLight() override = default;
	PointLight();
	static component_type GetType();
	static component_index GetTypeIndex();

	bool Init(const PointLightDesc& a_desc);

	const glm::vec3& GetColor() const;
	float GetPower() const;

	const Transform& GetTransform() const;
	Transform& GetTransform();
	glm::vec3 GetGlobalPosition() const;

private:
	glm::vec3 m_color;
	float m_power;

	Transform m_transform;
};
