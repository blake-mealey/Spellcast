#include "CubeTerrain.h"
#include "ContentManager.h"
#include "Logger.h"
#include "Uniforms.h"

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

	for (int x = 0; x < CX; ++x) {
		for (int z = 0; z < CZ; ++z) {
			const int height = 3 * (sin(x) + cos(z) + 2);
			for (int y = 0; y < height; ++y) {
				m_chunk.Set(x, y, z, y);
			}
		}
	}

	return m_chunk.Update();
}

void CubeTerrain::Render(const mat4& a_viewProjectionMatrix) {
	m_shader.Enable();

	if (m_material.HasAlbedoMaps()) {
		m_material.GetAlbedoMaps()->Bind(ALBEDO_TEXTURE_UNIT);
	}

	m_shader.SetModelViewProjectionMatrix(a_viewProjectionMatrix * mat4(1.f));
	m_shader.SetModelMatrix(mat4(1.f));
	m_chunk.Render();
}
