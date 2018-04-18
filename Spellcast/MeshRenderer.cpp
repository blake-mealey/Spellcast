#include "MeshRenderer.h"
#include "ContentManager.h"
#include "ShaderProgram.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

MeshRenderer::MeshRenderer() : m_mesh(nullptr) {}
MeshRenderer::~MeshRenderer() = default;

void MeshRenderer::SetEnabled(const bool a_enabled) {
	Component::SetEnabled(a_enabled);
}

component_type MeshRenderer::GetType() const {
	return Component::GetType() | ComponentType::MESH_RENDERER;
}

bool MeshRenderer::Init(MeshRendererDesc a_desc) {
	m_mesh = ContentManager::GetMesh(a_desc.m_meshPath);
	m_materials = move(a_desc.m_materials);

	return true;
}

void MeshRenderer::Render() const {
	if (!m_enabled) return;
	m_mesh->Render(this);
}

void MeshRenderer::InitRender(const size_t a_materialIndex) const {
	const MaterialPtr& material = m_materials[a_materialIndex];
	const ShaderProgramPtr& shader = material->GetShader();
	shader->Enable();

	shader->SetMaterial(material);

	const mat4 view = lookAt(vec3(0.f, 0.1f, -5.f), vec3(0.f), vec3(0.f, 1.f, 0.f));
	const mat4 projection = perspective(radians(60.f), 1024.f / 768.f, 0.1f, 1000.f);
	shader->SetModelAndViewAndProjectionMatrices(m_transform.GetTransformationMatrix(), view, projection);
}

Transform& MeshRenderer::GetTransform() {
	return m_transform;
}
