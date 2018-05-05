#include "ShadowShader.h"
#include "Uniforms.h"

using namespace glm;

bool ShadowShader::Init() {
	if (!ShaderProgram::Init()) return false;

	// ------------------------------------------------------------------------------------------
	// Compile and link shaders
	// ------------------------------------------------------------------------------------------
	
	if (!AddShader(GL_VERTEX_SHADER, "shadow.vert")) return false;
	if (!AddShader(GL_FRAGMENT_SHADER, "shadow.frag")) return false;
	if (!Finalize()) return false;

	// ------------------------------------------------------------------------------------------
	// Store uniform locations
	// ------------------------------------------------------------------------------------------

	m_modelViewProjectionMatrixLoc = GetUniformLocation("modelViewProjectionMatrix");
	m_depthBiasModelViewProjectionMatrixLoc = GetUniformLocation("depthBiasModelViewProjectionMatrix");
	
	m_shadowMapLoc = GetUniformLocation("shadowMap");
	m_intensityLoc = GetUniformLocation("intensity");
	
	m_powerLoc = GetUniformLocation("power");

	// ------------------------------------------------------------------------------------------
	// Load uniform defaults
	// ------------------------------------------------------------------------------------------

	Enable();

	SetShadowMapTextureUnit(SHADOW_TEXTURE_UNIT_INDEX);
	SetIntensity(0.75f);

	Disable();

	return true;
}

void ShadowShader::SetModelViewProjectionMatrix(const mat4& a_value) const {
	LoadUniform(m_modelViewProjectionMatrixLoc, a_value);
}

void ShadowShader::SetDepthBiasModelViewProjectionMatrix(const mat4& a_value) const {
	LoadUniform(m_depthBiasModelViewProjectionMatrixLoc, a_value);
}

void ShadowShader::SetShadowMapTextureUnit(const GLuint a_value) const {
	LoadUniform(m_shadowMapLoc, a_value);
}

void ShadowShader::SetIntensity(const float a_value) const {
	LoadUniform(m_intensityLoc, a_value);
}

void ShadowShader::SetPower(const float a_value) const {
	LoadUniform(m_powerLoc, a_value);
}
