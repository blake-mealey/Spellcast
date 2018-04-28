#pragma once

#include "Component.h"
#include "ContentManager.h"
#include "CubeMap.h"
#include "SkyboxShader.h"

#include <json/json.hpp>

class SkyboxRenderer;

struct SkyboxRendererDesc : ComponentDesc {
	SkyboxRendererDesc();
	explicit SkyboxRendererDesc(nlohmann::json& a_data);
	void Create(Entity* a_entity) override;

	CubeMap* m_cubeMap;
	Mesh* m_mesh;
	Texture* m_sunStrip;
};

class SkyboxRenderer : public Component {
friend SlotMap<SkyboxRenderer>;
public:
	~SkyboxRenderer() override = default;
	SkyboxRenderer();
	static component_type GetType();
	static component_index GetTypeIndex();

	bool Init(const SkyboxRendererDesc& a_desc);

	void Render(const glm::mat4& a_viewMatrix, const glm::mat4& a_projectionMatrix) const;

private:
	SkyboxShader m_shader;
	CubeMap* m_cubeMap;
	Mesh* m_mesh;
	Texture* m_sunStrip;
};
