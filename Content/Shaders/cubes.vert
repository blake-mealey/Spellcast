#version 430

layout(location = 0) in vec4 vertexPosition_model;
layout(location = 1) in vec3 vertexNormal_model;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewProjectionMatrix;

out vec3 fragmentPosition_camera;
out vec3 surfaceNormal_camera;
out vec3 eyeDirection_camera;

out vec4 worldCoord;
out vec4 texCoord;

void main() {
	gl_Position = modelViewProjectionMatrix * vec4(vertexPosition_model.xyz, 1);
	
	texCoord = vertexPosition_model;
	worldCoord = modelMatrix * vec4(vertexPosition_model.xyz, 1);

	vec3 vertexPosition_camera = (viewMatrix * modelMatrix * vec4(vertexPosition_model.xyz, 1)).xyz;
	eyeDirection_camera = -vertexPosition_camera;

	fragmentPosition_camera = vertexPosition_camera;

	surfaceNormal_camera = (viewMatrix * modelMatrix * vec4(vertexNormal_model, 0)).xyz;
}