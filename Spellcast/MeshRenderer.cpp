#include "MeshRenderer.h"
#include "ShaderProgram.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

MeshRenderer::MeshRenderer() : m_mesh(nullptr) {}

component_type MeshRenderer::GetType() {
	return Component::GetType() | ComponentType::MESH_RENDERER;
}

component_index MeshRenderer::GetTypeIndex() {
	return ComponentTypeIndex::MESH_RENDERER;
}

bool MeshRenderer::Init(const MeshRendererDesc& a_desc) {
	m_mesh = a_desc.m_mesh;
	m_transform = a_desc.m_transform;
	m_materials = a_desc.m_materials;

	return true;
}

void MeshRenderer::Render(const mat4& a_viewMatrix, const mat4& a_projectionMatrix) {
	if (!m_enabled) return;
	RenderContext::Render(a_viewMatrix, a_projectionMatrix);
	m_mesh->Render(this);
}

void MeshRenderer::InitRenderPass(const size_t& a_materialIndex) const {
	RenderContext::InitRenderPass(a_materialIndex);
	GetShader(a_materialIndex)->SetModelAndViewAndProjectionMatrices(
		m_transform.GetTransformationMatrix(), m_currentViewMatrix, m_currentProjectionMatrix);
}

Transform& MeshRenderer::GetTransform() {
	return m_transform;
}
