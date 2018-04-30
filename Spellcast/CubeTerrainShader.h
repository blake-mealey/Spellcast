#pragma once

#include "ShaderProgram.h"

class CubeTerrainShader : public ShaderProgram {
public:
	bool Init() override;

	void SetModelViewProjectionMatrix(const glm::mat4& a_value) const;
	void SetModelMatrix(const glm::mat4& a_value) const;
	void SetAlbedoMapsTextureUnit(GLuint a_value) const;

private:
	uniform_loc m_modelViewProjectionMatrixLoc = 0;
	uniform_loc m_modelMatrixLoc = 0;
	uniform_loc m_albedoMapsLoc = 0;
};
