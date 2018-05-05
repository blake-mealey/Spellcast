#pragma include "hsv.glsl"
#pragma include "lighting_helpers.glsl"

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

struct Material {
	vec4 albedoColor;
	vec4 specularColor;
	float specularity;
	float emission;
};

void lighting_set_ambient(const Material material, vec4 ambientColor) {
	fragmentColor = mix(ambientColor * material.albedoColor, material.albedoColor, material.emission);
}

void lighting_add_light(const Material material, vec3 lightDirection_camera, vec3 surfaceNormal_camera, vec3 eyeDirection_camera, vec4 lightColor, float attenuation) {
	vec3 l = normalize(lightDirection_camera);
	vec3 n = normalize(surfaceNormal_camera);
	vec3 E = normalize(eyeDirection_camera);
	vec3 R = reflect(-l, n);

	float cosTheta = clamp(dot(n, l), 0, 1);
	float cosAlpha = clamp(dot(E, R), 0, 1);
	
	vec4 lighting = (material.albedoColor * cosTheta * lightColor) +										// Diffuse
					(material.specularColor * lightColor * pow(cosAlpha, material.specularity));			// Specular

	fragmentColor += mix(attenuation * lighting, vec4(0), material.emission);
}

void lighting_add_direction_light(DirectionLight light, mat4 viewMatrix, vec3 surfaceNormal_camera, vec3 eyeDirection_camera, const Material material) {
	vec3 lightDirection_camera = (viewMatrix * vec4(-light.direction_world, 0)).xyz;
	lighting_add_light(material, lightDirection_camera, surfaceNormal_camera, eyeDirection_camera, vec4(light.color, 1.f), 1.f);
}

void lighting_add_spot_light(SpotLight light, mat4 viewMatrix, vec3 surfaceNormal_camera, vec3 eyeDirection_camera, vec3 fragmentPosition_camera, const Material material) {
	vec3 lightPosition_camera = (viewMatrix * vec4(light.position_world, 1)).xyz;
	vec3 lightDirection_camera = lightPosition_camera - fragmentPosition_camera;

	vec3 coneDirection_camera = (viewMatrix * vec4(normalize(light.direction_world), 0)).xyz;
	float lightAngle = acos(dot(-normalize(lightDirection_camera), coneDirection_camera));
	if (lightAngle < light.angle) {
		float attenuation = lighting_get_attenuation(light.power, length(lightDirection_camera));
		lighting_add_light(material, lightDirection_camera, surfaceNormal_camera, eyeDirection_camera, vec4(light.color, 1.f), attenuation);
	}
}

void lighting_add_point_light(PointLight light, mat4 viewMatrix, vec3 surfaceNormal_camera, vec3 eyeDirection_camera, vec3 fragmentPosition_camera, const Material material) {
	vec3 lightPosition_camera = (viewMatrix * vec4(light.position_world, 1)).xyz;
	vec3 lightDirection_camera = lightPosition_camera - fragmentPosition_camera;
	float attenuation = lighting_get_attenuation(light.power, length(lightDirection_camera));
	lighting_add_light(material, lightDirection_camera, surfaceNormal_camera, eyeDirection_camera, vec4(light.color, 1.f), attenuation);
}

void lighting_set_glow_color(const Material material, float glowScale) {
	float v = hsv_get_v(fragmentColor);
	glowColor = vec4((fragmentColor * (glowScale + material.emission * 0.5) * v * v).rgb, 1.f);
}
