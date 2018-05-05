#version 430

in vec3 fragmentPosition_camera;
in vec3 surfaceNormal_camera;
in vec3 eyeDirection_camera;
in vec2 fragmentUv;

out vec4 fragmentColor;
out vec4 glowColor;

#pragma include "lighting.glsl"

layout (std430, binding = 0) buffer directionLightData { DirectionLight directionLights[]; };
layout (std430, binding = 1) buffer spotLightData { SpotLight spotLights[]; };
layout (std430, binding = 2) buffer pointLightData { PointLight pointLights[]; };

uniform mat4 viewMatrix;

uniform Material material;

uniform vec4 ambientColor;

uniform sampler2D albedoMap;
uniform uint albedoMapEnabled;

uniform vec2 uvScale;

uniform float glowScale;

void main() {
	Material mat = material;
	mat.albedoColor = (1 - albedoMapEnabled) * mat.albedoColor
		+ albedoMapEnabled * texture(albedoMap, uvScale*vec2(1.f - fragmentUv.x, fragmentUv.y));

	lighting_set_ambient(mat, ambientColor);

	for (int i = 0; i < directionLights.length(); i++)
		lighting_add_direction_light(directionLights[i], viewMatrix, surfaceNormal_camera, eyeDirection_camera, mat);

	for (int i = 0; i < spotLights.length(); i++)
		lighting_add_spot_light(spotLights[i], viewMatrix, surfaceNormal_camera, eyeDirection_camera, fragmentPosition_camera, mat);
	
	for (int i = 0; i < pointLights.length(); i++)
		lighting_add_point_light(pointLights[i], viewMatrix, surfaceNormal_camera, eyeDirection_camera, fragmentPosition_camera, mat);

	lighting_set_glow_color(mat, glowScale);
}
