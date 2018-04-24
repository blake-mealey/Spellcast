#include "MeshRenderer.h"
#include "ShaderProgram.h"
#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;
using namespace nlohmann;

MeshRendererDesc::MeshRendererDesc() : m_mesh(nullptr), m_materials({}) {}

MeshRendererDesc::MeshRendererDesc(json& a_data): MeshRendererDesc() {
	m_mesh = ContentManager::GetMesh(ContentManager::FromJson<std::string>(a_data, "Mesh"));
	m_transform = Transform(a_data);
	if (a_data["Materials"].is_array()) {
		for (json& matData : a_data["Materials"]) {
			m_materials.push_back(ContentManager::GetMaterial(matData));
		}
	}
	if (m_materials.empty()) {
		json j = json::object();
		m_materials.push_back(ContentManager::GetMaterial(j));
	}
}

void MeshRendererDesc::Create(Entity* a_entity) {
	auto* renderer = World::CreateAndGetComponent<MeshRenderer>();
	renderer->Init(this);
	renderer->GetTransform().SetParent(&a_entity->GetTransform());
	a_entity->AddComponent(renderer);
}




MeshRenderer::MeshRenderer() : m_mesh(nullptr) {}

component_type MeshRenderer::GetType() {
	return Component::GetType() | (1 << GetTypeIndex());
}

component_index MeshRenderer::GetTypeIndex() {
	return ComponentTypeIndex::MESH_RENDERER;
}

bool MeshRenderer::Init(const MeshRendererDesc* a_desc) {
	m_mesh = a_desc->m_mesh;
	m_transform = a_desc->m_transform;
	m_materials = std::vector<Material*>(a_desc->m_materials);

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
