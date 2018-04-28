#version 430

#pragma include "includes/lighting.glsl"

in vec4 texCoord;

out vec4 fragmentColor;
out vec4 glowColor;

void main() {
	fragmentColor = vec4(texCoord.w / 128.0, texCoord.w / 256.0, texCoord.w / 512.0, 1.0);
	glowColor = lighting_glow_color(fragmentColor, 0.1, 0);
}