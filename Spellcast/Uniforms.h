#pragma once

#define INVALID_UNIFORM_LOCATION 0xffffffff

// Define texture unit and index for each type of texture
// to avoid hard-coding and extra code later

#define ALBEDO_TEXTURE_UNIT				GL_TEXTURE0
#define ALBEDO_TEXTURE_UNIT_INDEX		0

#define SHADOW_TEXTURE_UNIT				GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT_INDEX		1

#define SUN_STRIP_TEXTURE_UNIT			GL_TEXTURE3
#define SUN_STRIP_TEXTURE_UNIT_INDEX	3

#define SKYBOX_TEXTURE_UNIT				GL_TEXTURE4
#define SKYBOX_TEXTURE_UNIT_INDEX		4

