#version 430

const float BIAS = 0.005;

uniform sampler2DShadow shadowMap;
uniform float intensity;

in vec4 shadowCoord;

out vec4 fragmentColor;
out vec4 glowColor;

void main() {
	float visibility = 1.0 - (intensity * texture(shadowMap, vec3(shadowCoord.xy, shadowCoord.z - BIAS)));

	fragmentColor = vec4(vec3(visibility), 1.0);
	glowColor = fragmentColor;
}