#include "MeshRenderer.h"
#include "ContentManager.h"
#include "ShaderProgram.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

MeshRenderer::MeshRenderer() : m_mesh(nullptr) {}

component_type MeshRenderer::GetType() const {
	return Component::GetType() | ComponentType::MESH_RENDERER;
}

bool MeshRenderer::Init(const MeshRendererDesc& a_desc) {
	m_mesh = a_desc.m_mesh;
	m_materials = a_desc.m_materials;

	return true;
}

void MeshRenderer::Render(const mat4& a_viewMatrix, const mat4& a_projectionMatrix) const {
	if (!m_enabled) return;
	m_mesh->Render(this, a_viewMatrix, a_projectionMatrix);
}

void MeshRenderer::InitRender(const size_t a_materialIndex, const mat4& a_viewMatrix, const mat4& a_projectionMatrix) const {
	const MaterialPtr& material = m_materials[a_materialIndex];
	const ShaderProgramPtr& shader = material->GetShader();
	shader->Enable();

	shader->SetMaterial(material);
	shader->SetModelAndViewAndProjectionMatrices(m_transform.GetTransformationMatrix(), a_viewMatrix, a_projectionMatrix);
}

Transform& MeshRenderer::GetTransform() {
	return m_transform;
}
