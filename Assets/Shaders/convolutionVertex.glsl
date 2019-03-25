#version 400 core

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout (location = 0) in vec3 position;

out Vertex {
	vec3 normal;
} OUT;

void main()
{
    OUT.normal = position;  
	mat4 rotView = mat4(mat3(viewMatrix)); // remove translation from the view matrix
    gl_Position =  projMatrix * rotView * vec4(position, 1.0);
}