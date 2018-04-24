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

// NOTE:	To add a component type, add an enum item to the struct at any index and
//			then add a string to the array in ComponentType.cpp at the same index

// Unique, no-gap indices per component type
struct ComponentTypeIndex {
	enum {
		BASE_COMPONENT = 0,
		MESH_RENDERER,
		SKYBOX_RENDERER,
		CAMERA,
		COUNT
	};
};

struct ComponentType {
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
