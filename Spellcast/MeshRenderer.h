#pragma once

#include "Component.h"
#include "Mesh.h"
#include "ContentManager.h"
#include "Transform.h"
#include "RenderContext.h"

#include <json/json.hpp>

struct MeshRendererDesc {
	MeshRendererDesc() : m_mesh(nullptr), m_materials({}) {}

	explicit MeshRendererDesc(nlohmann::json& a_data) : MeshRendererDesc() {
		m_mesh = ContentManager::GetMesh(ContentManager::FromJson<std::string>(a_data, "Mesh"));
		m_transform = Transform(a_data);
		if (a_data["Materials"].is_array()) {
			for (nlohmann::json& matData : a_data["Materials"]) {
				m_materials.push_back(ContentManager::GetMaterial(matData));
			}
		}
		if (m_materials.empty()) {
			nlohmann::json j = nlohmann::json::object();
			m_materials.push_back(ContentManager::GetMaterial(j));
		}
	}

	MeshPtr m_mesh;
	Transform m_transform;
	std::vector<MaterialPtr> m_materials;
};

class MeshRenderer : public Component, RenderContext {
public:
	MeshRenderer();
	component_type GetType() const override;

	bool Init(const MeshRendererDesc& a_desc);

	void Render(const glm::mat4& a_viewMatrix, const glm::mat4& a_projectionMatrix) override;
	void InitRenderPass(const size_t& a_materialIndex) const override;

	Transform& GetTransform();

private:
	MeshPtr m_mesh;
	Transform m_transform;
};
