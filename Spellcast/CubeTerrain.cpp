#include "CubeTerrain.h"
#include "ContentManager.h"
#include "Logger.h"
#include "Uniforms.h"

#include <glm/gtc/noise.hpp>

using namespace nlohmann;
using namespace glm;

CubeTerrainDesc::CubeTerrainDesc() = default;

CubeTerrainDesc::CubeTerrainDesc(json& a_data) {}

void CubeTerrainDesc::Create(Entity* a_entity) {
	auto* terrain = World::CreateAndGetComponent<CubeTerrain>();
	if (!terrain->Init(*this)) Logger::Console()->warn("CubeTerrain init failed.");
	a_entity->AddComponent(terrain);
}




CubeTerrain::CubeTerrain() = default;

component_type CubeTerrain::GetType() {
	return Component::GetType() | (1 << GetTypeIndex());
}

component_index CubeTerrain::GetTypeIndex() {
	return ComponentTypeIndex::CUBE_TERRAIN;
}

bool CubeTerrain::Init(const CubeTerrainDesc& a_desc) {
	if (!m_shader.Init()) return false;
	if (!m_chunk.Init()) return false;
	if (!m_material.Init()) return false;

	m_chunk.Set(0, 0, 0, 20);

	constexpr float scale = 0.07f;
	for (int x = 0; x < CX; ++x) {
		for (int z = 0; z < CZ; ++z) {
			// const int height = 3 * (sin(x) + cos(z) + 2);
			const int height = 1 + 10 * (simplex(vec2(x, z) * scale) + 1.f) / 2.f;
			for (int y = 0; y < height; ++y) {
				m_chunk.Set(x, y, z, y);
			}
		}
	}

	return m_chunk.Update();
}

void CubeTerrain::Render(const mat4& a_viewMatrix, const mat4& a_viewProjectionMatrix) {
	m_shader.Enable();

	if (m_material.HasAlbedoMaps()) {
		m_material.GetAlbedoMaps()->Bind(ALBEDO_TEXTURE_UNIT);
	}

	const mat4 modelMatrix(1.f);
	m_shader.SetModelViewProjectionMatrix(a_viewProjectionMatrix * modelMatrix);
	m_shader.SetModelMatrix(modelMatrix);
	m_shader.SetViewMatrix(a_viewMatrix);
	m_chunk.Render();
}
