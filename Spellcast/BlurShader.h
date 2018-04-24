#pragma once

#include "ShaderProgram.h"

class BlurShader : public ShaderProgram {
public:
	bool Init() override;

	void SetModelMatrix(const glm::mat4& a_value) const;
	void SetTextureUnit(GLuint a_value) const;
	void SetOffset(const glm::vec2& a_value) const;

private:
	uniform_loc m_modelMatrixLoc = 0;
	uniform_loc m_screenLoc = 0;
	uniform_loc m_offsetLoc = 0;
};
