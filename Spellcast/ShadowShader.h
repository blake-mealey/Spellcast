#pragma once

#include "ShaderProgram.h"

class ShadowShader : public ShaderProgram {
public:
	bool Init() override;

	void SetDepthModelViewProjectionMatrix(const glm::mat4& a_value) const;

private:
	uniform_loc m_depthModelViewProjectionMatrixLoc = 0;
};
