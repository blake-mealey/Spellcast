#include "LightingShader.h"
#include "Uniforms.h"
#include "Material.h"
#include "Texture.h"

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
	m_depthBiasModelViewProjectionMatrixLoc = GetUniformLocation("depthBiasModelViewProjectionMatrix");

	m_materialDiffuseColorLoc = GetUniformLocation("materialDiffuseColor");
	m_materialSpecularColorLoc = GetUniformLocation("materialSpecularColor");
	m_materialSpecularityLoc = GetUniformLocation("materialSpecularity");
	m_materialEmissivenessLoc = GetUniformLocation("materialEmissiveness");
	
	m_ambientColorLoc = GetUniformLocation("ambientColor");
	
	m_shadowMapLoc = GetUniformLocation("shadowMap");
	m_shadowsEnabledLoc = GetUniformLocation("shadowsEnabled");
	
	m_diffuseTextureLoc = GetUniformLocation("diffuseTexture");
	m_diffuseTextureEnabledLoc = GetUniformLocation("diffuseTextureEnabled");
	
	m_uvScaleLoc = GetUniformLocation("uvScale");
	
	m_bloomScaleLoc = GetUniformLocation("bloomScale");

	// ------------------------------------------------------------------------------------------
	// Load uniform defaults
	// ------------------------------------------------------------------------------------------

	Enable();

	const mat4 model = mat4(1.f);
	const mat4 view = lookAt(vec3(0.f, 0.1f, -5.f), vec3(0.f), vec3(0.f, 1.f, 0.f));
	const mat4 projection = perspective(radians(60.f), 1024.f / 768.f, 0.1f, 1000.f);

	SetModelMatrix(model);
	SetViewMatrix(view);
	SetModelViewProjectionMatrix(projection * view * model);

	SetDiffuseTextureUnit(ALBEDO_TEXTURE_UNIT_INDEX);
	SetShadowMapTextureUnit(SHADOW_TEXTURE_UNIT_INDEX);

	SetDiffuseTextureEnabled(false);
	SetShadowsEnabled(false);

	SetMaterialColor(vec4(195.f, 144.f, 212.f, 255.f) / 255.f);
	SetMaterialSpecularColor(vec4(51.f, 51.f, 51.f, 255.f) / 255.f);
	SetMaterialSpecularity(1.f);
	SetMaterialEmission(0.f);

	SetAmbientColor(vec4(0.4f, 0.4f, 0.4f, 1.f));

	SetUvScale(vec2(1.f));

	SetBloomScale(0.1f);

	Disable();

	LoadLights({DirectionLight(vec3(1.f), vec3(0.f, -1.f, -2.f))}, {}, {});

	return true;
}

void LightingShader::SetMaterial(const Material* a_material) const {
	SetMaterialColor(a_material->GetColor());
	SetMaterialSpecularColor(a_material->GetSpecularColor());
	SetMaterialSpecularity(a_material->GetSpecularity());
	SetMaterialEmission(a_material->GetEmission());
	
	SetDiffuseTextureEnabled(a_material->HasAlbedoMap());

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

void LightingShader::SetDepthBiasModelViewProjectionMatrix(const mat4& a_value) const {
	LoadUniform(m_depthBiasModelViewProjectionMatrixLoc, a_value);
}

void LightingShader::SetMaterialColor(const vec4& a_value) const {
	LoadUniform(m_materialDiffuseColorLoc, a_value);
}

void LightingShader::SetMaterialSpecularColor(const vec4& a_value) const {
	LoadUniform(m_materialSpecularColorLoc, a_value);
}

void LightingShader::SetMaterialSpecularity(const float a_value) const {
	LoadUniform(m_materialSpecularityLoc, a_value);
}

void LightingShader::SetMaterialEmission(const float a_value) const {
	LoadUniform(m_materialEmissivenessLoc, a_value);
}

void LightingShader::SetAmbientColor(const vec4& a_value) const {
	LoadUniform(m_ambientColorLoc, a_value);
}

void LightingShader::SetShadowMapTextureUnit(const GLuint a_value) const {
	LoadUniform(m_shadowMapLoc, a_value);
}

void LightingShader::SetShadowsEnabled(const bool a_value) const {
	LoadUniform(m_shadowsEnabledLoc, a_value);
}

void LightingShader::SetDiffuseTextureUnit(const GLuint a_value) const {
	LoadUniform(m_diffuseTextureLoc, a_value);
}

void LightingShader::SetDiffuseTextureEnabled(const bool a_value) const {
	LoadUniform(m_diffuseTextureEnabledLoc, a_value);
}

void LightingShader::SetUvScale(const vec2& a_value) const {
	LoadUniform(m_uvScaleLoc, a_value);
}

void LightingShader::SetBloomScale(const float a_value) const {
	LoadUniform(m_bloomScaleLoc, a_value);
}

void LightingShader::LoadLights(const std::vector<DirectionLight>& a_directionLights,
	const std::vector<SpotLight>& a_spotLights, const std::vector<PointLight>& a_pointLights) const {
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbos[SSBOs::DirectionLights]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, a_directionLights.size() * sizeof(DirectionLight), a_directionLights.data(), GL_DYNAMIC_COPY);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbos[SSBOs::SpotLights]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, a_spotLights.size() * sizeof(SpotLight), a_spotLights.data(), GL_DYNAMIC_COPY);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbos[SSBOs::PointLights]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, a_pointLights.size() * sizeof(PointLight), a_pointLights.data(), GL_DYNAMIC_COPY);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}