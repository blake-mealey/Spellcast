#pragma once

#include "Component.h"
#include "Mesh.h"

#include <json/json.hpp>
#include "ContentManager.h"
#include "Transform.h"

struct MeshRendererDesc {
	explicit MeshRendererDesc(const nlohmann::json& a_data) {
		m_meshPath = ContentManager::FromJson<std::string>(a_data, "Mesh");
		if (a_data["Materials"].is_array()) {
			for (const nlohmann::json& matData : a_data["Materials"]) {
				m_materials.push_back(ContentManager::GetMaterial(matData));
			}
		}
		if (m_materials.empty()) {
			m_materials.push_back(ContentManager::GetMaterial(nlohmann::json::object()));
		}
	}

	std::string m_meshPath;
	std::vector<MaterialPtr> m_materials;
};

class MeshRenderer : public Component {
public:
	MeshRenderer();
	~MeshRenderer() override;
	void SetEnabled(bool a_enabled) override;
	component_type GetType() const override;

	bool Init(MeshRendererDesc a_desc);

	void Render() const;
	void InitRender(size_t a_materialIndex) const;

	Transform& GetTransform();

private:
	MeshPtr m_mesh;
	std::vector<MaterialPtr> m_materials;
	Transform m_transform;
};
