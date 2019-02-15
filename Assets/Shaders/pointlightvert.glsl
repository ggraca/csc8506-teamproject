#version 400 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 shadowMatrix;

in vec3 position;
out mat4 inverseProjView;

void main (void) {
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);

	inverseProjView = inverse(projMatrix * viewMatrix);
}