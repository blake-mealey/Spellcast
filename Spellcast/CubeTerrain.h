#pragma once

#include "Component.h"
#include "CubeChunk.h"
#include "CubeTerrainShader.h"

#include <json/json.hpp>
#include "TerrainMaterial.h"

// Based on the tutorials from: https://en.wikibooks.org/wiki/OpenGL_Programming/Glescraft_1

class CubeTerrainDesc : public ComponentDesc {
public:
	CubeTerrainDesc();
	explicit CubeTerrainDesc(nlohmann::json& a_data);
	void Create(Entity* a_entity) override;
};

class CubeTerrain : public Component {
friend SlotMap<CubeTerrain>;
public:
	~CubeTerrain() override = default;
	CubeTerrain();
	static component_type GetType();
	static component_index GetTypeIndex();

	bool Init(const CubeTerrainDesc& a_desc);

	void Render(const glm::mat4& a_viewMatrix, const glm::mat4& a_viewProjectionMatrix);

private:
	TerrainMaterial m_material;

	CubeTerrainShader m_shader;
	CubeChunk m_chunk;
};
