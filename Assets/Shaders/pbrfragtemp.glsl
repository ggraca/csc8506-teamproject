#version 400 core

uniform sampler2D mainTex; // Diffuse texture map
uniform sampler2D bumpTex; // Bump map
uniform sampler2D specularTex;
uniform sampler2D metalnessTex;

uniform samplerCube cubeTex;
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
	vec3 normal = IN.normal;
	vec4 metalness = vec4(1.0, 0.0, 0.0, 1.0);

	vec3 incident = normalize(IN.worldPos - cameraPos);
	vec4 reflection = vec4(1.0, 1.0, 1.0, 1.0);
	
	fragColour [0] = texture2D(mainTex , IN.texCoord) * objectColour;
	fragColour [1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
	fragColour [2] = vec4(0.0, 1.0, 0.0, 1.0);
}