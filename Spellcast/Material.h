#pragma once

#include "ContentManager.h"

#include <json/json.hpp>

#include <memory>

class Material;
typedef std::shared_ptr<Material> MaterialPtr;

class Material {
public:
	Material();
	~Material();

	static MaterialPtr Create();

	bool Init(const nlohmann::json& a_data);
	
	// void Use() const;
	const ShaderProgramPtr& GetShader() const;

	bool HasAlbedoMap() const;
	const TexturePtr& GetAlbedoMap() const;
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
	ShaderProgramPtr m_shader;
	
	TexturePtr m_albedoMap;
	glm::vec4 m_color;

	TexturePtr m_specularMap;
	glm::vec4 m_specularColor;
	float m_specularity;

	float m_emission;

	TexturePtr m_normalMap;
	TexturePtr m_heightMap;

	glm::vec2 m_uvScale;
	glm::vec2 m_uvOffset;
};
