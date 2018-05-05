#version 430

in vec4 fragmentPosition;
in vec4 shadowCoord;

out vec4 fragmentColor;
out vec4 glowColor;

#pragma include "lighting_helpers.glsl"

const float BIAS = 0.005;

uniform sampler2DShadow shadowMap;
uniform float intensity;

uniform float power;

void main() {
	float visibility = 1.f;

	if (shadowCoord.x/shadowCoord.w >= 0 && shadowCoord.x/shadowCoord.w <= 1 && shadowCoord.y/shadowCoord.w >= 0 && shadowCoord.y/shadowCoord.w <= 1) {
		visibility -= intensity * lighting_get_attenuation_safe(power, length(shadowCoord.xyz - fragmentPosition.xyz)) * texture(shadowMap, vec3(shadowCoord.xy, shadowCoord.z - BIAS) / shadowCoord.w);
	}

	fragmentColor = vec4(vec3(visibility), 1.0);
	glowColor = fragmentColor;
}