#pragma once

#include "ArrayTexture.h"

class TerrainMaterial {
public:
	TerrainMaterial();
	bool Init();
	
	bool HasAlbedoMaps() const;
	const ArrayTexture* GetAlbedoMaps() const;

private:
	ArrayTexture* m_albedoMaps;
};
