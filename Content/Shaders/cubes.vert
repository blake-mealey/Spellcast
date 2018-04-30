#version 430

layout(location = 0) in vec4 coord_model;

uniform mat4 modelMatrix;
uniform mat4 modelViewProjectionMatrix;

out vec4 worldCoord;
out vec4 texCoord;

void main() {
	texCoord = coord_model;
	worldCoord = modelMatrix * vec4(coord_model.xyz, 1);
	gl_Position = modelViewProjectionMatrix * vec4(coord_model.xyz, 1);
}