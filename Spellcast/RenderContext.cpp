#include "RenderContext.h"
#include "ShaderProgram.h"

void RenderContext::Render(const glm::mat4& a_viewMatrix, const glm::mat4& a_projectionMatrix) {
	m_currentViewMatrix = a_viewMatrix;
	m_currentProjectionMatrix = a_projectionMatrix;
	m_currentViewProjectionMatrix = a_projectionMatrix * a_viewMatrix;
}

void RenderContext::InitRenderPass(const size_t& a_materialIndex) const {
	const MaterialPtr& material = m_materials[a_materialIndex];
	const ShaderProgramPtr& shader = material->GetShader();
	shader->Enable();
	shader->SetMaterial(material);
}

const ShaderProgramPtr& RenderContext::GetShader(const size_t& a_materialIndex) const {
	return m_materials[a_materialIndex]->GetShader();
}
