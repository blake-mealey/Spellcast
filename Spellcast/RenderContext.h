#pragma once

#include "Material.h"

#include <glm/glm.hpp>

class RenderContext {
public:
	virtual void Render(const glm::mat4& a_viewMatrix, const glm::mat4& a_projectionMatrix);
	virtual void InitRenderPass(const size_t& a_materialIndex) const;

protected:
	const ShaderProgramPtr& GetShader(const size_t& a_materialIndex) const;

	std::vector<MaterialPtr> m_materials;

	glm::mat4 m_currentViewMatrix;
	glm::mat4 m_currentProjectionMatrix;
	glm::mat4 m_currentViewProjectionMatrix;
};
