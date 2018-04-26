#include "MeshRenderer.h"
#include "ShaderProgram.h"
#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;
using namespace nlohmann;

MeshRendererDesc::MeshRendererDesc() : m_mesh(nullptr), m_materials({}), m_castsShadows(false),
                                       m_receivesShadows(false) {}

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

	m_castsShadows = ContentManager::FromJson(a_data, "CastsShadows", true);
	m_receivesShadows = ContentManager::FromJson(a_data, "ReceivesShadows", true);
}

void MeshRendererDesc::Create(Entity* a_entity) {
	auto* renderer = World::CreateAndGetComponent<MeshRenderer>();
	renderer->Init(this);
	renderer->GetTransform().SetParent(&a_entity->GetTransform());
	a_entity->AddComponent(renderer);
}




MeshRenderer::MeshRenderer() : m_mesh(nullptr), m_castsShadows(false), m_receivesShadows(false) {}

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

	m_castsShadows = a_desc->m_castsShadows;
	m_receivesShadows = a_desc->m_receivesShadows;

	return true;
}

void MeshRenderer::Render(const mat4& a_viewMatrix, const mat4& a_projectionMatrix) const {
	if (!m_enabled) return;
	// mat4 viewProjectionMatrix = a_projectionMatrix * a_viewMatrix;
	for (const Mesh::Entry& entry : m_mesh->GetEntries()) {
		const Material* material = m_materials[entry.m_materialIndex];
		const ShaderProgram* shader = material->GetShader();
		shader->Enable();
		shader->SetMaterial(material);
		shader->SetModelAndViewAndProjectionMatrices(m_transform.GetTransformationMatrix(),
			a_viewMatrix, a_projectionMatrix);

		entry.Render();
	}
}

void MeshRenderer::RenderBasic() const {
	if (!m_enabled) return;
	m_mesh->RenderBasic();
}

Transform& MeshRenderer::GetTransform() {
	return m_transform;
}

const Transform& MeshRenderer::GetTransform() const {
	return m_transform;
}

bool MeshRenderer::DoesCastShadows() const {
	return m_castsShadows;
}

bool MeshRenderer::DoesReceiveShadows() const {
	return m_receivesShadows;
}
