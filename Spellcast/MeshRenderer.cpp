#include "MeshRenderer.h"
#include "ContentManager.h"

using namespace std;

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

const MaterialPtr& MeshRenderer::GetMaterial(size_t a_index) const {
	return m_materials[a_index];
}
