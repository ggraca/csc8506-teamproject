#version 400 core

uniform sampler2D diffuseTex;
uniform sampler2D materialTex;
uniform sampler2D emissiveTex;
uniform sampler2D lightSpecularTex;
uniform sampler2D KDTex;

uniform vec4 ambientColour;

const float PI = 3.14159265359;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColour;

void main (void) {
	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	diffuse.x = pow(diffuse.x, 2.2f);
	diffuse.y = pow(diffuse.y, 2.2f);
	diffuse.z = pow(diffuse.z, 2.2f);
	float ao = texture(diffuseTex, IN.texCoord).b;
	vec4 radiance = texture(emissiveTex, IN.texCoord);
	vec4 specular = texture(lightSpecularTex, IN.texCoord);
	vec4 KDCol = texture(KDTex, IN.texCoord);
	vec3 kDiffuse = KDCol.rgb;
	float NdotL = KDCol.a;
	
	//Checking if skybox
	if(kDiffuse == vec3(0.0f) && radiance == vec4(1.0f)){
		fragColour = diffuse;
		return;
	}
	
	vec3 lightOutput = ((kDiffuse * diffuse.xyz / PI) + specular.xyz) * radiance.xyz * NdotL;
	vec3 ambient = ambientColour.xyz * diffuse.xyz * ao;
	
	vec3 finalCol = ambient + lightOutput;
	finalCol = finalCol / (finalCol + vec3(1.0f));
	
	fragColour = vec4(finalCol, 1.0f);
}
