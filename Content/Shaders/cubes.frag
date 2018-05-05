#version 430

in vec3 fragmentPosition_camera;
in vec3 surfaceNormal_camera;
in vec3 eyeDirection_camera;

in vec4 worldCoord;
in vec4 texCoord;

out vec4 fragmentColor;
out vec4 glowColor;

#pragma include "lighting.glsl"

layout (std430, binding = 0) buffer directionLightData { DirectionLight directionLights[]; };
layout (std430, binding = 1) buffer spotLightData { SpotLight spotLights[]; };
layout (std430, binding = 2) buffer pointLightData { PointLight pointLights[]; };

uniform mat4 viewMatrix;

const Material material = { vec4(1, 0, 0, 1), vec4(0), 0, 0 };

uniform vec4 ambientColor;

uniform sampler2DArray albedoMaps;

const float uvScale = 5.f;
const float glowScale = 0.1;

void main() {
	Material mat = material;
	if (texCoord.w > 0)
		mat.albedoColor = texture(albedoMaps, vec3(vec2(worldCoord.x + worldCoord.z, worldCoord.y) / uvScale, abs(texCoord.w)));
	else
		mat.albedoColor = texture(albedoMaps, vec3(vec2(worldCoord.x / 3.f, worldCoord.z / 3.f) / uvScale, abs(texCoord.w)));

	lighting_set_ambient(mat, ambientColor);

	for (int i = 0; i < directionLights.length(); i++)
		lighting_add_direction_light(directionLights[i], viewMatrix, surfaceNormal_camera, eyeDirection_camera, mat);

	for (int i = 0; i < spotLights.length(); i++)
		lighting_add_spot_light(spotLights[i], viewMatrix, surfaceNormal_camera, eyeDirection_camera, fragmentPosition_camera, mat);
	
	for (int i = 0; i < pointLights.length(); i++)
		lighting_add_point_light(pointLights[i], viewMatrix, surfaceNormal_camera, eyeDirection_camera, fragmentPosition_camera, mat);

	lighting_set_glow_color(mat, glowScale);
}