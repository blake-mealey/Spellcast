#include "CopyShader.h"
#include "Uniforms.h"

using namespace glm;

bool CopyShader::Init() {
	if (!ShaderProgram::Init()) return false;

	// ------------------------------------------------------------------------------------------
	// Compile and link shaders
	// ------------------------------------------------------------------------------------------
	
	if (!AddShader(GL_VERTEX_SHADER, "copy.vert")) return false;
	if (!AddShader(GL_FRAGMENT_SHADER, "copy.frag")) return false;
	if (!Finalize()) return false;

	// ------------------------------------------------------------------------------------------
	// Store uniform locations
	// ------------------------------------------------------------------------------------------

	m_modelMatrixLoc = GetUniformLocation("modelMatrix");
	m_screenLoc = GetUniformLocation("screen");

	// ------------------------------------------------------------------------------------------
	// Load uniform defaults
	// ------------------------------------------------------------------------------------------

	Enable();

	SetTextureUnit(ALBEDO_TEXTURE_UNIT_INDEX);
	SetModelMatrix(mat4(1.f));

	Disable();

	return true;
}

void CopyShader::SetModelMatrix(const mat4& a_value) const {
	LoadUniform(m_modelMatrixLoc, a_value);
}

void CopyShader::SetTextureUnit(const GLuint a_value) const {
	LoadUniform(m_screenLoc, a_value);
}
