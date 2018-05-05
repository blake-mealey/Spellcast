#include "LightingShader.h"
#include "Uniforms.h"
#include "Material.h"
#include "Texture.h"
#include "World.h"
#include "DirectionLight.h"
#include "SpotLight.h"
#include "PointLight.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

LightingShader::~LightingShader() {
	// TODO: Safe delete?
	glDeleteBuffers(SSBOs::Count, m_ssbos);
}

bool LightingShader::Init() {
	if (!ShaderProgram::Init()) return false;

	// ------------------------------------------------------------------------------------------
	// Compile and link shaders
	// ------------------------------------------------------------------------------------------
	
	if (!AddShader(GL_VERTEX_SHADER, "lighting.vert")) return false;
	if (!AddShader(GL_FRAGMENT_SHADER, "lighting.frag")) return false;
	if (!Finalize()) return false;

	// ------------------------------------------------------------------------------------------
	// Generate and initialize storage buffers
	// ------------------------------------------------------------------------------------------

	glGenBuffers(SSBOs::Count, m_ssbos);
	for (size_t i = 0; i < SSBOs::Count; ++i) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, m_ssbos[i]);
	}

	// ------------------------------------------------------------------------------------------
	// Store uniform locations
	// ------------------------------------------------------------------------------------------

	m_modelMatrixLoc = GetUniformLocation("modelMatrix");
	m_viewMatrixLoc = GetUniformLocation("viewMatrix");
	m_modelViewProjectionMatrixLoc = GetUniformLocation("modelViewProjectionMatrix");

	m_materialAlbedoColorLoc = GetUniformLocation("material.albedoColor");
	m_materialSpecularColorLoc = GetUniformLocation("material.specularColor");
	m_materialSpecularityLoc = GetUniformLocation("material.specularity");
	m_materialEmissionLoc = GetUniformLocation("material.emission");
	
	m_ambientColorLoc = GetUniformLocation("ambientColor");
	
	m_albedoMapLoc = GetUniformLocation("albedoMap");
	m_albedoMapEnabledLoc = GetUniformLocation("albedoMapEnabled");
	
	m_uvScaleLoc = GetUniformLocation("uvScale");
	
	m_glowScaleLoc = GetUniformLocation("glowScale");

	// ------------------------------------------------------------------------------------------
	// Load uniform defaults
	// ------------------------------------------------------------------------------------------

	Enable();

	SetAlbedoMapUnit(ALBEDO_TEXTURE_UNIT_INDEX);
	SetAlbedoMapEnabled(false);

	SetAmbientColor(vec4(0.4f, 0.4f, 0.4f, 1.f));

	SetGlowScale(0.1f);

	Disable();

	return true;
}

void LightingShader::SetMaterial(const Material* a_material) const {
	SetMaterialColor(a_material->GetColor());
	SetMaterialSpecularColor(a_material->GetSpecularColor());
	SetMaterialSpecularity(a_material->GetSpecularity());
	SetMaterialEmission(a_material->GetEmission());
	
	SetAlbedoMapEnabled(a_material->HasAlbedoMap());

	SetUvScale(a_material->GetUvScale());
	// SetUvOffset(a_material->GetUvOffset());

	// SetBloomScale(a_material->GetBloomScale());

	if (a_material->HasAlbedoMap()) a_material->GetAlbedoMap()->Bind(ALBEDO_TEXTURE_UNIT);
}

void LightingShader::SetModelAndViewAndProjectionMatrices(const mat4& a_modelMatrix, const mat4& a_viewMatrix, const mat4& a_projectionMatrix) const {
	SetModelMatrix(a_modelMatrix);
	SetViewMatrix(a_viewMatrix);
	SetModelViewProjectionMatrix(a_projectionMatrix * a_viewMatrix * a_modelMatrix);
}

void LightingShader::SetModelMatrix(const mat4& a_value) const {
	LoadUniform(m_modelMatrixLoc, a_value);
}

void LightingShader::SetViewMatrix(const mat4& a_value) const {
	LoadUniform(m_viewMatrixLoc, a_value);
}

void LightingShader::SetModelViewProjectionMatrix(const mat4& a_value) const {
	LoadUniform(m_modelViewProjectionMatrixLoc, a_value);
}

void LightingShader::SetMaterialColor(const vec4& a_value) const {
	LoadUniform(m_materialAlbedoColorLoc, a_value);
}

void LightingShader::SetMaterialSpecularColor(const vec4& a_value) const {
	LoadUniform(m_materialSpecularColorLoc, a_value);
}

void LightingShader::SetMaterialSpecularity(const float a_value) const {
	LoadUniform(m_materialSpecularityLoc, a_value);
}

void LightingShader::SetMaterialEmission(const float a_value) const {
	LoadUniform(m_materialEmissionLoc, a_value);
}

void LightingShader::SetAmbientColor(const vec4& a_value) const {
	LoadUniform(m_ambientColorLoc, a_value);
}

void LightingShader::SetAlbedoMapUnit(const GLuint a_value) const {
	LoadUniform(m_albedoMapLoc, a_value);
}

void LightingShader::SetAlbedoMapEnabled(const bool a_value) const {
	LoadUniform(m_albedoMapEnabledLoc, a_value);
}

void LightingShader::SetUvScale(const vec2& a_value) const {
	LoadUniform(m_uvScaleLoc, a_value);
}

void LightingShader::SetGlowScale(const float a_value) const {
	LoadUniform(m_glowScaleLoc, a_value);
}

void LightingShader::LoadLights(const vector<DirectionLightData>& a_directionLights,
	const vector<SpotLightData>& a_spotLights, const vector<PointLightData>& a_pointLights) const {
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbos[SSBOs::DirectionLights]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, a_directionLights.size() * sizeof(DirectionLightData), a_directionLights.data(), GL_DYNAMIC_COPY);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbos[SSBOs::SpotLights]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, a_spotLights.size() * sizeof(SpotLightData), a_spotLights.data(), GL_DYNAMIC_COPY);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbos[SSBOs::PointLights]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, a_pointLights.size() * sizeof(PointLightData), a_pointLights.data(), GL_DYNAMIC_COPY);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightingShader::LoadLights() const {
	vector<DirectionLightData> directionLights;
	vector<SpotLightData> spotLights;
	vector<PointLightData> pointLights;

	Enable();
	
	for (auto it = World::BeginComponents<DirectionLight>(); it != World::EndComponents<DirectionLight>(); ++it) {
		const DirectionLight& light = *it;
		if (!light.IsEnabled()) continue;
		directionLights.emplace_back(light.GetColor(), light.GetDirection());
	}

	for (auto it = World::BeginComponents<SpotLight>(); it != World::EndComponents<SpotLight>(); ++it) {
		const SpotLight& light = *it;
		if (!light.IsEnabled()) continue;
		spotLights.emplace_back(light.GetColor(), light.GetPower(), light.GetGlobalPosition(),
		                        light.GetAngleRadians(), light.GetGlobalDirection());
	}

	for (auto it = World::BeginComponents<PointLight>(); it != World::EndComponents<PointLight>(); ++it) {
		const PointLight& light = *it;
		if (!light.IsEnabled()) continue;
		pointLights.emplace_back(light.GetColor(), light.GetPower(), light.GetGlobalPosition());
	}

	LoadLights(directionLights, spotLights, pointLights);

	Disable();
}
