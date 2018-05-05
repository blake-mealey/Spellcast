#include "CubeTerrainShader.h"
#include "Uniforms.h"

using namespace glm;

bool CubeTerrainShader::Init() {
	if (!ShaderProgram::Init()) return false;

	// ------------------------------------------------------------------------------------------
	// Compile and link shaders
	// ------------------------------------------------------------------------------------------
	
	if (!AddShader(GL_VERTEX_SHADER, "cubes.vert")) return false;
	if (!AddShader(GL_FRAGMENT_SHADER, "cubes.frag")) return false;
	if (!Finalize()) return false;

	// ------------------------------------------------------------------------------------------
	// Store uniform locations
	// ------------------------------------------------------------------------------------------

	m_modelViewProjectionMatrixLoc = GetUniformLocation("modelViewProjectionMatrix");
	m_modelMatrixLoc = GetUniformLocation("modelMatrix");
	m_viewMatrixLoc = GetUniformLocation("viewMatrix");
	
	m_ambientColorLoc = GetUniformLocation("ambientColor");

	m_albedoMapsLoc = GetUniformLocation("albedoMaps");

	// ------------------------------------------------------------------------------------------
	// Load uniform defaults
	// ------------------------------------------------------------------------------------------

	Enable();
	
	SetAmbientColor(vec4(0.4f, 0.4f, 0.4f, 1.f));
	SetAlbedoMapsTextureUnit(ALBEDO_TEXTURE_UNIT_INDEX);

	Disable();

	return true;
}

void CubeTerrainShader::SetModelViewProjectionMatrix(const mat4& a_value) const {
	LoadUniform(m_modelViewProjectionMatrixLoc, a_value);
}

void CubeTerrainShader::SetModelMatrix(const glm::mat4& a_value) const {
	LoadUniform(m_modelMatrixLoc, a_value);
}

void CubeTerrainShader::SetViewMatrix(const mat4& a_value) const {
	LoadUniform(m_viewMatrixLoc, a_value);
}

void CubeTerrainShader::SetAmbientColor(const vec4& a_value) const {
	LoadUniform(m_ambientColorLoc, a_value);
}

void CubeTerrainShader::SetAlbedoMapsTextureUnit(const GLuint a_value) const {
	LoadUniform(m_albedoMapsLoc, a_value);
}
