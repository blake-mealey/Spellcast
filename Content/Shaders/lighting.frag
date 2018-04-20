#version 430

#pragma include "includes/hsv.glsl"

struct PointLight {
	vec3 color;
	float power;
	vec3 position_world;
};

struct DirectionLight {
	vec3 color;
	vec3 direction_world;
};

struct SpotLight {
	vec3 color;
	float power;
	vec3 position_world;
	float angle;
	vec3 direction_world;
};

layout (std430, binding = 0) buffer directionLightData { DirectionLight directionLights[]; };
layout (std430, binding = 1) buffer spotLightData { SpotLight spotLights[]; };
layout (std430, binding = 2) buffer pointLightData { PointLight pointLights[]; };

uniform mat4 viewMatrix;

uniform vec4 materialDiffuseColor;
uniform vec4 materialSpecularColor;
uniform float materialSpecularity;
uniform float materialEmissiveness;

uniform vec4 ambientColor;

uniform sampler2DShadow shadowMap;
uniform uint shadowsEnabled;

uniform sampler2D diffuseTexture;
uniform uint diffuseTextureEnabled;

uniform vec2 uvScale;

uniform float bloomScale;

in vec3 fragmentPosition_camera;
in vec3 surfaceNormal_camera;
in vec3 eyeDirection_camera;
in vec2 fragmentUv;
in vec4 shadowCoord;

out vec4 fragmentColor;
out vec4 glowColor;

vec4 getColorFromLight(vec4 diffuseColor, vec3 lightDirection_camera, vec4 lightColor) {
	vec3 n = normalize(surfaceNormal_camera);
	vec3 l = normalize(lightDirection_camera);
	vec3 E = normalize(eyeDirection_camera);
	vec3 R = reflect(-l, n);

	float cosTheta = clamp(dot(n, l), 0, 1);
	float cosAlpha = clamp(dot(E, R), 0, 1);
	
	return (diffuseColor * cosTheta * lightColor) +										// Diffuse
		   (materialSpecularColor * lightColor * pow(cosAlpha, materialSpecularity));	// Specular
}

void main() {
	// float bias = 0.005 * tan(acos(dot(surfaceNormal_camera, l)));
	// bias = clamp(bias, 0, 0.01);
	float bias = 0.005;
	float visibility = 1.0;
	visibility -= shadowsEnabled * (0.75 * texture(shadowMap, vec3(shadowCoord.xy, shadowCoord.z - bias)));

	vec4 diffuseColor = (1 - diffuseTextureEnabled) * materialDiffuseColor
		+ diffuseTextureEnabled * texture(diffuseTexture, uvScale*vec2(1.f - fragmentUv.x, fragmentUv.y));
	vec4 materialAmbientColor = ambientColor * diffuseColor;

	fragmentColor = mix(materialAmbientColor, diffuseColor, materialEmissiveness);
	
	for (int i = 0; i < pointLights.length(); i++) {
		PointLight light = pointLights[i];
		vec3 lightPosition_camera = (viewMatrix * vec4(light.position_world, 1)).xyz;
		vec3 lightDirection_camera = lightPosition_camera - fragmentPosition_camera;
		float distanceToLight = length(lightDirection_camera);
		float attenuation = 1.0 / (1.0 * (1.0/light.power) * (distanceToLight*distanceToLight));
		fragmentColor += mix(visibility * attenuation * getColorFromLight(diffuseColor, lightDirection_camera, vec4(light.color, 1.f)), vec4(0.f), materialEmissiveness);
	}

	for (int i = 0; i < directionLights.length(); i++) {
		DirectionLight light = directionLights[i];
		vec3 lightDirection_camera = (viewMatrix * vec4(-light.direction_world, 0)).xyz;
		fragmentColor += mix(visibility * getColorFromLight(diffuseColor, lightDirection_camera, vec4(light.color, 1.f)), vec4(0.f), materialEmissiveness);
	}

	for (int i = 0; i < spotLights.length(); i++) {
		SpotLight light = spotLights[i];
		vec3 lightPosition_camera = (viewMatrix * vec4(light.position_world, 1)).xyz;
		vec3 lightDirection_camera = lightPosition_camera - fragmentPosition_camera;

		vec3 coneDirection_camera = (viewMatrix * vec4(normalize(light.direction_world), 0)).xyz;
		float lightAngle = acos(dot(-normalize(lightDirection_camera), coneDirection_camera));
		if (lightAngle < light.angle) {
			float distanceToLight = length(lightDirection_camera);
			float attenuation = 1.0 / (1.0 * (1.0/light.power) * (distanceToLight*distanceToLight));
			fragmentColor += mix(visibility * attenuation * getColorFromLight(diffuseColor, lightDirection_camera, vec4(light.color, 1.f)), vec4(0.f), materialEmissiveness);
		}
	}

	vec3 hsv = hsv_from_rgb(fragmentColor);
	glowColor = fragmentColor * (bloomScale + materialEmissiveness * 0.5) * hsv.z * hsv.z;
	glowColor = vec4(glowColor.rgb, 1.f);
}