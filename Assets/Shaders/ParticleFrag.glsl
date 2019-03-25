#version 400 core

uniform sampler2D diffuseTex;
uniform vec3 objectColour;

in Vertex {
	vec2 texCoord;
	float lifetime;
} IN;

out vec4 fragColor;

void main(void){
	float alphaFromLifeTime = 1.0f;
	if (IN.lifetime < 0.2f){
		alphaFromLifeTime = IN.lifetime * 5.0f;
	}
	else if (IN.lifetime > 0.8f) {
		alphaFromLifeTime = (1.0f - IN.lifetime) * 5.0f;
	}
	fragColor = texture(diffuseTex, IN.texCoord);
	fragColor.r *= objectColour.r;
	fragColor.g *= objectColour.g;
	fragColor.b *= objectColour.b;
	fragColor.a *= alphaFromLifeTime;
	fragColor.rgb *= fragColor.a;
}