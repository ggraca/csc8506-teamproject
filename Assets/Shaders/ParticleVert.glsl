#version 400 core

uniform vec4 worldPosition;
uniform mat4 identity;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;

out mat4 ModelMatrix;

out Vertex {
	vec2 texCoord;
} OUT;

void main (void) {
	OUT.texCoord = texCoord;
	
	//Create modelMatrix from position and scale
	mat4 modelMatrix = identity;
	float scale = worldPosition.w;
	
	modelMatrix[3] = vec4(worldPosition.xyz, 1.0f);
	modelMatrix[0][0] = scale;
	modelMatrix[1][1] = scale;
	modelMatrix[2][2] = scale;
	
	mat4 MV = (viewMatrix * modelMatrix);

	//After MV calculation, remove all rotations in upper 3x3
	MV[0] = vec4(worldPosition.w, 0.0f, 0.0f, MV[0][3]);
	MV[1] = vec4(0.0f, worldPosition.w, 0.0f, MV[1][3]);
	MV[2] = vec4(0.0f, 0.0f, worldPosition.w, MV[2][3]);
	
	
	vec4 pos = MV * vec4(position, 1.0);
	gl_Position = projMatrix * pos;
}