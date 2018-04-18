#pragma once

typedef size_t component_type;

struct ComponentType {
	static const component_type BASE_COMPONENT =					1 << 0;
	static const component_type MESH_RENDERER =						1 << 1;
	static const component_type CAMERA =							1 << 2;
};
