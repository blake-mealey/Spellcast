#include "BlurShader.h"
#include "Uniforms.h"

using namespace glm;

bool BlurShader::Init() {
	if (!ShaderProgram::Init()) return false;

	// ------------------------------------------------------------------------------------------
	// Compile and link shaders
	// ------------------------------------------------------------------------------------------

	if (!AddShader(GL_VERTEX_SHADER, "copy.vert")) return false;
	if (!AddShader(GL_FRAGMENT_SHADER, "blur.frag")) return false;
	if (!Finalize()) return false;

	// ------------------------------------------------------------------------------------------
	// Store uniform locations
	// ------------------------------------------------------------------------------------------

	m_modelMatrixLoc = GetUniformLocation("modelMatrix");
	m_screenLoc = GetUniformLocation("screen");
	m_offsetLoc = GetUniformLocation("offset");

	// ------------------------------------------------------------------------------------------
	// Load uniform defaults
	// ------------------------------------------------------------------------------------------

	Enable();

	SetTextureUnit(ALBEDO_TEXTURE_UNIT_INDEX);
	SetModelMatrix(mat4(1.f));
	SetOffset(vec2(0.f));

	Disable();

	return true;
}

void BlurShader::SetModelMatrix(const mat4& a_value) const {
	LoadUniform(m_modelMatrixLoc, a_value);
}

void BlurShader::SetTextureUnit(const GLuint a_value) const {
	LoadUniform(m_screenLoc, a_value);
}

void BlurShader::SetOffset(const vec2& a_value) const {
	LoadUniform(m_offsetLoc, a_value);
}