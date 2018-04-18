#pragma once

#include "Component.h"
#include "Mesh.h"
#include "ContentManager.h"
#include "Transform.h"

#include <json/json.hpp>

struct MeshRendererDesc {
	explicit MeshRendererDesc(nlohmann::json& a_data) {
		m_meshPath = ContentManager::FromJson<std::string>(a_data, "Mesh");
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

	std::string m_meshPath;
	std::vector<MaterialPtr> m_materials;
};

class MeshRenderer : public Component {
public:
	MeshRenderer();
	component_type GetType() const override;

	bool Init(const MeshRendererDesc& a_desc);

	void Render() const;
	void InitRender(size_t a_materialIndex) const;

	Transform& GetTransform();

private:
	MeshPtr m_mesh;
	std::vector<MaterialPtr> m_materials;
	Transform m_transform;
};
