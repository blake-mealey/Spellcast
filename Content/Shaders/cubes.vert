#version 430

layout(location = 0) in vec4 coord_model;

uniform mat4 modelViewProjectionMatrix;

out vec4 texCoord;

void main() {
	texCoord = coord_model;
	gl_Position = modelViewProjectionMatrix * vec4(coord_model.xyz, 1);
}