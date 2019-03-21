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
    gl_Position =  projection * view * vec4(normal, 1.0);
}