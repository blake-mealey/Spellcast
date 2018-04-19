#pragma once

#include "Component.h"
#include "ContentManager.h"
#include "Transform.h"
#include "RenderContext.h"

#include <json/json.hpp>

struct MeshRendererDesc : ComponentDesc {
	MeshRendererDesc();
	explicit MeshRendererDesc(nlohmann::json& a_data);
	Component* Create() override;

	MeshPtr m_mesh;
	Transform m_transform;
	std::vector<MaterialPtr> m_materials;
};

class MeshRenderer : public Component, RenderContext {
public:
	MeshRenderer();
	static component_type GetType();
	static component_index GetTypeIndex();

	bool Init(const MeshRendererDesc& a_desc);

	void Render(const glm::mat4& a_viewMatrix, const glm::mat4& a_projectionMatrix) override;
	void InitRenderPass(const size_t& a_materialIndex) const override;

	Transform& GetTransform();

private:
	MeshPtr m_mesh;
	Transform m_transform;
};
