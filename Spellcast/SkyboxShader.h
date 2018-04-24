#pragma once

#include "ShaderProgram.h"

class SkyboxShader : public ShaderProgram {
public:
	bool Init() override;

	void SetViewProjectionMatrix(const glm::mat4& a_value) const;

	void SetSunSizeRadians(float a_value) const;
	void SetSunDirection(const glm::vec3& a_value) const;
	void SetSunTextureUnit(GLuint a_value) const;

	void SetColorAdjust(const glm::vec3& a_value) const;
	void SetSkyboxTextureUnit(GLuint a_value) const;
	
	void SetTime(float a_value) const;

private:
	uniform_loc m_viewProjectionMatrixLoc = 0;

	uniform_loc m_sunSizeRadiansLoc = 0;
	uniform_loc m_sunDirectionLoc = 0;
	uniform_loc m_sunLoc = 0;

	uniform_loc m_colorAdjustLoc = 0;
	uniform_loc m_skyboxLoc = 0;
	
	uniform_loc m_timeLoc = 0;
};
