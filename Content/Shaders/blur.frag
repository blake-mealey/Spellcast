#version 430

in vec2 fragmentUv;

uniform vec2 offset;
uniform sampler2D screen;

out vec4 fragmentColor;

void main() {
	vec4 c = vec4(0.0);
	c += 5.0/16.0 * texture(screen, fragmentUv - offset);
	c += 6.0/16.0 * texture(screen, fragmentUv);
	c += 5.0/16.0 * texture(screen, fragmentUv + offset);
	fragmentColor = c;
}