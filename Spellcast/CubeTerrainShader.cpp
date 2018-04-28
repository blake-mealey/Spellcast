#include "CubeTerrainShader.h"

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

	// ------------------------------------------------------------------------------------------
	// Load uniform defaults
	// ------------------------------------------------------------------------------------------

	Enable();

	SetModelViewProjectionMatrix(mat4(1.f));

	Disable();

	return true;
}

void CubeTerrainShader::SetModelViewProjectionMatrix(const mat4& a_value) const {
	LoadUniform(m_modelViewProjectionMatrixLoc, a_value);
}
