#version 430

#pragma include "hsv.glsl"

in vec3 fragmentUv;

uniform float sunSizeRadians;
uniform vec3 sunDirection;
uniform sampler2D sun;

uniform vec3 colorAdjust;
uniform samplerCube skybox;

uniform float time;

out vec4 fragmentColor;
out vec4 glowColor;

void main() {
	// Get the adjusted skybox colour
	fragmentColor = texture(skybox, fragmentUv) * vec4(colorAdjust, 1);

	// Make the stars sparkle
	vec3 fragmentHsv = hsv_from_rgb(fragmentColor);
	float brightnessModifier = sin(time) * 0.3 * (fragmentHsv.z*fragmentHsv.z);
	fragmentHsv += vec3(0, 0, brightnessModifier + 2*abs(brightnessModifier));
	fragmentColor = vec4(hsv_to_rgb(fragmentHsv), fragmentColor.w);

	// Blend with sun colour
	float sunSize = sunSizeRadians + sin(time) * 0.02;
	float angle = acos(dot(normalize(-sunDirection), normalize(fragmentUv)));
	if (angle < sunSize - 0.01) {
		vec2 sunPos = vec2(angle/sunSize, 0.5);
		vec4 sunColor = texture(sun, sunPos);
		fragmentColor = (sunColor*(2 - pow(sunPos.x,1.5)) * sunColor.w) + (fragmentColor * (1 - sunColor.w));
	}

	vec3 hsv = hsv_from_rgb(fragmentColor);
	glowColor = vec4(fragmentColor.rgb * 0.25 * hsv.z * hsv.z, fragmentColor.a);
}
