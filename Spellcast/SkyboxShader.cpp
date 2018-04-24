#include "SkyboxShader.h"
#include "Uniforms.h"

using namespace glm;

bool SkyboxShader::Init() {
	if (!ShaderProgram::Init()) return false;

	// ------------------------------------------------------------------------------------------
	// Compile and link shaders
	// ------------------------------------------------------------------------------------------
	
	if (!AddShader(GL_VERTEX_SHADER, "skybox.vert")) return false;
	if (!AddShader(GL_FRAGMENT_SHADER, "skybox.frag")) return false;
	if (!Finalize()) return false;

	// ------------------------------------------------------------------------------------------
	// Store uniform locations
	// ------------------------------------------------------------------------------------------

	m_viewProjectionMatrixLoc = GetUniformLocation("viewProjectionMatrix");

	m_sunSizeRadiansLoc = GetUniformLocation("sunSizeRadians");
	m_sunDirectionLoc = GetUniformLocation("sunDirection");
	m_sunLoc = GetUniformLocation("sun");
	
	m_colorAdjustLoc = GetUniformLocation("colorAdjust");
	m_skyboxLoc = GetUniformLocation("skybox");
	
	m_timeLoc = GetUniformLocation("time");

	// ------------------------------------------------------------------------------------------
	// Load uniform defaults
	// ------------------------------------------------------------------------------------------

	Enable();
	
	SetColorAdjust(vec3(1.f));
	SetSunTextureUnit(SUN_STRIP_TEXTURE_UNIT_INDEX);
	SetSkyboxTextureUnit(SKYBOX_TEXTURE_UNIT_INDEX);
	SetSunDirection(vec3(0.f, -1.f, -2.f));
	SetSunSizeRadians(0.1f);
	SetTime(0.f);

	Disable();

	return true;
}

void SkyboxShader::SetViewProjectionMatrix(const mat4& a_value) const {
	LoadUniform(m_viewProjectionMatrixLoc, a_value);
}

void SkyboxShader::SetSunSizeRadians(const float a_value) const {
	LoadUniform(m_sunSizeRadiansLoc, a_value);
}

void SkyboxShader::SetSunDirection(const vec3& a_value) const {
	LoadUniform(m_sunDirectionLoc, a_value);
}

void SkyboxShader::SetSunTextureUnit(const GLuint a_value) const {
	LoadUniform(m_sunLoc, a_value);
}

void SkyboxShader::SetColorAdjust(const vec3& a_value) const {
	LoadUniform(m_colorAdjustLoc, a_value);
}

void SkyboxShader::SetSkyboxTextureUnit(const GLuint a_value) const {
	LoadUniform(m_skyboxLoc, a_value);
}

void SkyboxShader::SetTime(const float a_value) const {
	LoadUniform(m_timeLoc, a_value);
}
