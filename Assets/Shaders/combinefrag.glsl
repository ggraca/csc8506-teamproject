#version 400 core

uniform sampler2D diffuseTex;
uniform sampler2D emissiveTex;
uniform sampler2D lightSpecularTex;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColour;

void main (void) {
	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	vec4 light = texture(emissiveTex, IN.texCoord);
	vec4 lightSpecular = texture(lightSpecularTex, IN.texCoord);

	vec4 finalCol = diffuse;
	finalCol *= light;
	finalCol += lightSpecular;
	finalCol.a = 1.0f;
	
	fragColour = finalCol;
}
