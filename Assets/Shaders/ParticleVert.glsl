#version 400 core

uniform vec4 worldPosition[500];
uniform mat4 identity;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform float particleLifeTime[500];
uniform float particleMaxLifeTime;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;

out Vertex {
	vec2 texCoord;
	float lifetime;
} OUT;

void main (void) {
	OUT.texCoord = texCoord;
	OUT.lifetime = particleLifeTime[gl_InstanceID] / particleMaxLifeTime;
	
	//Create modelMatrix from position and scale
	mat4 modelMatrix = identity;
	vec4 instancePos = worldPosition[gl_InstanceID];
	float scale = instancePos.w;
	
	modelMatrix[3] = vec4(instancePos.xyz, 1.0f);
	modelMatrix[0][0] = scale;
	modelMatrix[1][1] = scale;
	modelMatrix[2][2] = scale;
	
	mat4 MV = (viewMatrix * modelMatrix);

	//After MV calculation, remove all rotations in upper 3x3
	MV[0] = vec4(scale, 0.0f, 0.0f, MV[0][3]);
	MV[1] = vec4(0.0f, scale, 0.0f, MV[1][3]);
	MV[2] = vec4(0.0f, 0.0f, scale, MV[2][3]);
	
	
	vec4 pos = MV * vec4(position, 1.0);
	gl_Position = projMatrix * pos;
}