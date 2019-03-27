#version 400 core

uniform sampler2D diffuseTex; // Diffuse texture map
uniform sampler2D bumpTex; // Bump map
uniform sampler2D roughnessTex;
uniform sampler2D metalnessTex;
uniform sampler2D aoTex;

uniform vec3 cameraPos;
uniform vec4 objectColour;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
} IN;

out vec4 fragColour [3]; // Our final outputted colours !

void main (void) {
	mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
	vec3 normal = normalize(TBN * normalize((texture2D(bumpTex, vec2(IN.texCoord.x, 1 - IN.texCoord.y)).rgb ) * 2.0 - 1.0));
	
	float roughness = texture(roughnessTex, vec2(IN.texCoord.x, 1 - IN.texCoord.y)).r;
	float metalness = texture(metalnessTex, vec2(IN.texCoord.x, 1 - IN.texCoord.y)).r;
	float ao = texture(aoTex, vec2(IN.texCoord.x, 1 - IN.texCoord.y)).r;

	fragColour [0] = texture2D(diffuseTex, vec2(IN.texCoord.x, 1 - IN.texCoord.y)) * objectColour;
	fragColour [1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
	fragColour [2] = vec4(roughness, metalness, ao, 1.0);
}