#version 430

#pragma include "includes/lighting.glsl"

const float uvScale = 5.f;

uniform sampler2DArray albedoMaps;

in vec4 worldCoord;
in vec4 texCoord;

out vec4 fragmentColor;
out vec4 glowColor;

void main() {
	if (texCoord.w > 0)
		fragmentColor = texture(albedoMaps, vec3(vec2(worldCoord.x + worldCoord.z, worldCoord.y) / uvScale, abs(texCoord.w)));
	else
		fragmentColor = texture(albedoMaps, vec3(vec2(worldCoord.x / 3.f, worldCoord.z / 3.f) / uvScale, abs(texCoord.w)));
	glowColor = lighting_glow_color(fragmentColor, 0.1, 0);
}