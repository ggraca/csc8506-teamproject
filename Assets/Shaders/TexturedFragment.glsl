#version 400 core

uniform sampler2D diffuseTex;
uniform bool isDepth = false;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColor;

void main(void){
	vec4 col = texture(diffuseTex, IN.texCoord);
	if (isDepth){
		col.g = col.r;
		col.b = col.r;
		col.a = 1.0f;
	}
	fragColor = col;
}