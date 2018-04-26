#version 430

layout(location = 0) in vec3 vertexPosition_model;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 depthBiasModelViewProjectionMatrix;

out vec4 shadowCoord;

void main() {
	gl_Position = modelViewProjectionMatrix * vec4(vertexPosition_model, 1);
	shadowCoord = depthBiasModelViewProjectionMatrix * vec4(vertexPosition_model, 1);
}
