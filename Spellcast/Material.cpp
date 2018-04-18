#include "Material.h"
#include "ShaderProgram.h"
#include "Uniforms.h"
#include "Texture.h"

using namespace std;
using namespace glm;

Material::Material(): m_specularity(0), m_emission(0) {}
Material::~Material() = default;

MaterialPtr Material::Create() {
	return make_shared<Material>();
}

bool Material::Init(const nlohmann::json& a_data) {
	m_shader = ContentManager::GetShaderProgram(ContentManager::FromJson<string>(a_data, "Shader", "Lighting"));
	
	m_albedoMap = ContentManager::GetTexture(ContentManager::FromJson<string>(a_data, "AlbedoMap"));
	m_color = ContentManager::ColorFromJson(a_data, "Color", ContentManager::COLOR_WHITE);

	// TODO: specular map
	m_specularColor = ContentManager::ColorFromJson(a_data, "SpecularColor", ContentManager::COLOR_WHITE);
	m_specularity = ContentManager::FromJson(a_data, "Specularity", 0.5f);
	
	m_emission = ContentManager::FromJson(a_data, "Emission", 0.f);

	// TODO: normal map
	// TODO: height map

	m_uvScale = ContentManager::VecFromJson(a_data, "UvScale", vec2(1.f));
	m_uvOffset = ContentManager::VecFromJson(a_data, "UvOffset", vec2(0.f));

	return true;
}

const ShaderProgramPtr& Material::GetShader() const {
	return m_shader;
}

/*void Material::Use() const {
	m_shader->Enable();
	m_shader->SetMaterial(this);
}*/

bool Material::HasAlbedoMap() const {
	return m_albedoMap != nullptr;
}

const TexturePtr& Material::GetAlbedoMap() const {
	return m_albedoMap;
}

const vec4& Material::GetColor() const {
	return m_color;
}

bool Material::HasSpecularMap() const {
	return m_specularMap != nullptr;
}

const vec4& Material::GetSpecularColor() const {
	return m_specularColor;
}

float Material::GetSpecularity() const {
	return m_specularity;
}

float Material::GetEmission() const {
	return m_emission;
}

bool Material::HasNormalMap() const {
	return m_normalMap != nullptr;
}

bool Material::HasHeightMap() const {
	return m_heightMap != nullptr;
}

const vec2& Material::GetUvScale() const {
	return m_uvScale;
}

const vec2& Material::GetUvOffset() const {
	return m_uvOffset;
}
