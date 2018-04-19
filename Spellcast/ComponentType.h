#pragma once

#include <string>

#define INVALID_COMPONENT_INDEX -1
#define INVALID_COMPONENT_TYPE -1

typedef long long component_type;
typedef int component_index;

template <class T>
struct ComponentTraits {
	static component_type GetType() { return T::GetType(); }
	static component_index GetTypeIndex() { return T::GetTypeIndex(); }
};

// Unique, no-gap indices per component type
struct ComponentTypeIndex {
	enum {
		BASE_COMPONENT = 0,
		MESH_RENDERER,
		CAMERA,
		COUNT
	};
};

struct ComponentType {
	// Unique bit flag per component type
	// TODO: Is this even needed? It is directly derived from index
	static constexpr component_type BASE_COMPONENT =					1 << ComponentTypeIndex::BASE_COMPONENT;
	static constexpr component_type MESH_RENDERER =						1 << ComponentTypeIndex::MESH_RENDERER;
	static constexpr component_type CAMERA =							1 << ComponentTypeIndex::CAMERA;

	// Display names per component type
	static const std::string DISPLAY_NAMES[ComponentTypeIndex::COUNT];
	
	static component_index GetIndex(const std::string& a_name);

	template <class T>
	static std::string GetDisplayName();

	template <class T>
	static std::string GetDisplayName(T* a_component);
};

template <class T>
std::string ComponentType::GetDisplayName() {
	return DISPLAY_NAMES[ComponentTraits<T>::GetTypeIndex()];
}

template <class T>
std::string ComponentType::GetDisplayName(T* a_component) {
	return DISPLAY_NAMES[ComponentTraits<T>::GetTypeIndex()];
}
