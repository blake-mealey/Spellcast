#include "ComponentType.h"

const std::string ComponentType::DISPLAY_NAMES[ComponentTypeIndex::COUNT] = {
	"BaseComponent",
	"MeshRenderer",
	"SkyboxRenderer",
	"Camera",
	"DirectionLight",
	"SpotLight",
	"PointLight"
};

component_index ComponentType::GetIndex(const std::string& a_name) {
	for (component_index i = 0; i < ComponentTypeIndex::COUNT; ++i) {
		if (DISPLAY_NAMES[i] == a_name) return i;
	}
	return INVALID_COMPONENT_INDEX;
}
