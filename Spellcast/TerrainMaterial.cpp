#include "TerrainMaterial.h"
#include "ContentManager.h"

TerrainMaterial::TerrainMaterial(): m_albedoMaps(nullptr) {}

bool TerrainMaterial::Init() {
	m_albedoMaps = new ArrayTexture();
	return m_albedoMaps->LoadFromFile(ContentManager::GetContentPath("Albedo.png", "Textures/Terrain/"));
}

bool TerrainMaterial::HasAlbedoMaps() const {
	return m_albedoMaps != nullptr;
}

const ArrayTexture* TerrainMaterial::GetAlbedoMaps() const {
	return m_albedoMaps;
}
