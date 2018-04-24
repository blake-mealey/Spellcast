#pragma once

#include "ShaderProgram.h"
#include "Texture.h"

#include <json/json.hpp>

class Material {
public:
	Material();
	~Material();

	bool Init(nlohmann::json& a_data);
	
	const ShaderProgram* GetShader() const;

	bool HasAlbedoMap() const;
	const Texture* GetAlbedoMap() const;
	const glm::vec4& GetColor() const;

	bool HasSpecularMap() const;
	const glm::vec4& GetSpecularColor() const;
	float GetSpecularity() const;

	float GetEmission() const;

	bool HasNormalMap() const;
	bool HasHeightMap() const;

	const glm::vec2& GetUvScale() const;
	const glm::vec2& GetUvOffset() const;

private:
	ShaderProgram* m_shader;
	
	Texture* m_albedoMap;
	glm::vec4 m_color;

	Texture* m_specularMap;
	glm::vec4 m_specularColor;
	float m_specularity;

	float m_emission;

	Texture* m_normalMap;
	Texture* m_heightMap;

	glm::vec2 m_uvScale;
	glm::vec2 m_uvOffset;
};
