#pragma include "includes/hsv.glsl"

vec4 lighting_glow_color(vec4 fragmentColor, float glowScale, float emission) {
	float v = hsv_get_v(fragmentColor);
	return vec4((fragmentColor * (glowScale + emission * 0.5) * v * v).rgb, 1.f);
}