#pragma once

#include "ShaderProgram.h"

class ShadowShader : public ShaderProgram {
public:
	bool Init() override;

	void SetModelViewProjectionMatrix(const glm::mat4& a_value) const;
	void SetDepthBiasModelViewProjectionMatrix(const glm::mat4& a_value) const;
	
	void SetShadowMapTextureUnit(GLuint a_value) const;
	void SetIntensity(float a_value) const;
	
	void SetPower(float a_value) const;

private:
	uniform_loc m_modelViewProjectionMatrixLoc = 0;
	uniform_loc m_depthBiasModelViewProjectionMatrixLoc = 0;
	
	uniform_loc m_shadowMapLoc = 0;
	uniform_loc m_intensityLoc = 0;
	
	uniform_loc m_powerLoc = 0;
};
