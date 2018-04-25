#pragma once

#include "Component.h"
#include "Texture.h"
#include "ShadowShader.h"

#include <json/json.hpp>
#include <glm/glm.hpp>

class DirectionLight;

struct DirectionLightDesc : ComponentDesc {
	DirectionLightDesc();
	explicit DirectionLightDesc(nlohmann::json& a_data);
	void Create(Entity* a_entity) override;

	glm::vec3 m_color;
	glm::vec3 m_direction;
	
	bool m_castsShadows;
	int m_shadowMapSize;
};

class DirectionLight : public Component {
	friend SlotMap<DirectionLight>;
public:
	~DirectionLight() override;
	DirectionLight();
	static component_type GetType();
	static component_index GetTypeIndex();

	bool Init(const DirectionLightDesc* a_desc);
	bool InitShadowMap();
	
	void RenderShadowMap();

	void SetCastsShadows(bool a_castsShadows);

	const Texture& GetShadowMap() const;
	const glm::mat4& GetDepthViewProjectionMatrix() const;
	
	const glm::vec3& GetColor() const;
	const glm::vec3& GetDirection() const;
	
	bool CastsShadows() const;

private:
	ShadowShader m_shadowShader;
	Texture m_shadowMap;
	GLuint m_shadowBuffer;
	glm::mat4 m_depthViewProjectionMatrix;

	glm::vec3 m_color;
	glm::vec3 m_direction;
	
	bool m_castsShadows;
	int m_shadowMapSize;
};
