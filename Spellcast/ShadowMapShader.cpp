#include "ShadowMapShader.h"

using namespace glm;

bool ShadowMapShader::Init() {
	if (!ShaderProgram::Init()) return false;

	// ------------------------------------------------------------------------------------------
	// Compile and link shaders
	// ------------------------------------------------------------------------------------------
	
	if (!AddShader(GL_VERTEX_SHADER, "shadow_map.vert")) return false;
	if (!AddShader(GL_FRAGMENT_SHADER, "shadow_map.frag")) return false;
	if (!Finalize()) return false;

	// ------------------------------------------------------------------------------------------
	// Store uniform locations
	// ------------------------------------------------------------------------------------------

	m_depthModelViewProjectionMatrixLoc = GetUniformLocation("depthModelViewProjectionMatrix");

	// ------------------------------------------------------------------------------------------
	// Load uniform defaults
	// ------------------------------------------------------------------------------------------

	Enable();

	SetDepthModelViewProjectionMatrix(mat4(1.f));

	Disable();

	return true;
}

void ShadowMapShader::SetDepthModelViewProjectionMatrix(const glm::mat4& a_value) const {
	LoadUniform(m_depthModelViewProjectionMatrixLoc, a_value);
}
