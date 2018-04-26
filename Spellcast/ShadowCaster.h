#pragma once

#include "glm/glm.hpp"
#include "Texture.h"
#include "ShadowMapShader.h"
#include "ShadowShader.h"

#define DEFAULT_SHADOW_MAP_SIZE 1024

class ShadowCaster {
public:
	virtual ~ShadowCaster();
	ShadowCaster();

	virtual bool InitShadowMap();

	virtual void RenderShadowMap();
	virtual void RenderShadows(const glm::mat4& a_viewProjectionMatrix) const;

	virtual void ComputeDepthViewProjectionMatrix() = 0;

	void SetCastsShadows(bool a_castsShadows);

	const Texture& GetShadowMap() const;
	const glm::mat4& GetDepthViewProjectionMatrix() const;

	bool CastsShadows() const;

protected:
	const static glm::mat4 BIAS_MATRIX;

	ShadowMapShader m_shadowMapShader;
	ShadowShader m_shadowShader;
	Texture m_shadowMap;
	GLuint m_shadowBuffer;
	glm::mat4 m_depthViewProjectionMatrix;
	
	float m_shadowIntensity;
	bool m_castsShadows;
	int m_shadowMapSize;
};