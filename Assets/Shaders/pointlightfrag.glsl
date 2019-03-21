#version 400 core

uniform sampler2D diffuseTex;
uniform sampler2D depthTex;
uniform sampler2D normTex;
uniform sampler2D materialTex;
uniform sampler2DShadow shadowTex;

uniform vec2 pixelSize;
uniform vec3 cameraPos;
in mat4 inverseProjView;
uniform mat4 shadowMatrix;
uniform bool drawShadows;

uniform float lightRadius;
uniform float lightBrightness;
uniform vec3 lightPos;
uniform vec4 lightColour;

const float PI = 3.14159265359;

out vec4 fragColour [3];

float DistributionGGX(vec3 Normal, vec3 HalfDir, float Roughness);
float GeometrySchlickGGX(float NdotV, float Roughness);
float GeometrySmith(vec3 Normal, vec3 ViewDir, vec3 LightDir, float Roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);

void main (void) {	
	vec3 pos = vec3((gl_FragCoord.x * pixelSize.x ), (gl_FragCoord.y * pixelSize.y), 0.0);
	pos.z = texture(depthTex, pos.xy ).r;
	
	vec3 normal = normalize(texture(normTex, pos.xy).xyz * 2.0 - 1.0);
	vec4 material = texture(materialTex, pos.xy);
	vec3 diffuse = texture(diffuseTex, pos.xy).xyz;
	diffuse.x = pow(diffuse.x, 2.2f);
	diffuse.y = pow(diffuse.y, 2.2f);
	diffuse.z = pow(diffuse.z, 2.2f);
	
	vec4 clip = inverseProjView * vec4(pos * 2.0 - 1.0, 1.0);
	pos = clip.xyz / clip.w;

	float roughness = material.r;
	float metalness = material.g;
	float ao = material.b;

	vec3 lightDir = normalize(lightPos - pos);
	vec3 viewDir = normalize(cameraPos - pos);
	vec3 halfDir = normalize(lightDir + viewDir);
	
	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, diffuse, metalness);
	
	float distance = length(lightPos - pos);
	distance = (1.0f - clamp(distance / lightRadius, 0.0f, 1.0f));
	float attenuation = distance * distance;
	
	if (attenuation == 0.0f){
		discard;
	}
	
	vec3 radiance = lightColour.xyz * lightBrightness * attenuation;

	float NDF = DistributionGGX(normal, halfDir, roughness);
	float G = GeometrySmith(normal, viewDir, lightDir, roughness);
	vec3 F = FresnelSchlick(max(dot(halfDir, viewDir), 0.0f), F0);
	
	vec3 kSpecular = F;
    vec3 kDiffuse = vec3(1.0) - kSpecular;
    kDiffuse *= 1.0 - metalness;	  
        
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0f * max(dot(normal, viewDir), 0.0f) * max(dot(normal, lightDir), 0.0f);
    vec3 specular     = numerator / max(denominator, 0.001f);
	
	float NdotL = max(dot(normal, lightDir), 0.0f);
	
	fragColour [0] = vec4(radiance, 1.0f);
	fragColour [1] = vec4(specular, 1.0f);
	fragColour [2] = vec4(kDiffuse, NdotL);
}

float DistributionGGX(vec3 Normal, vec3 HalfDir, float Roughness)
{
    float a      = Roughness * Roughness;
    float a2     = a * a;
    float NdotH  = max(dot(Normal, HalfDir), 0.0f);
    float NdotH2 = NdotH * NdotH;
	
    float numerator   = a2;
    float denominator = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denominator = PI * denominator * denominator;
	
    return numerator / denominator;
}

float GeometrySchlickGGX(float NdotV, float Roughness)
{
    float r = (Roughness + 1.0f);
    float k = (r * r) / 8.0f;

    float numerator   = NdotV;
    float denominator = NdotV * (1.0f - k) + k;
	
    return numerator / denominator;
}

float GeometrySmith(vec3 Normal, vec3 ViewDir, vec3 LightDir, float Roughness)
{
    float NdotV = max(dot(Normal, ViewDir), 0.0f);
    float NdotL = max(dot(Normal, LightDir), 0.0f);
    float ggx2  = GeometrySchlickGGX(NdotV, Roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, Roughness);
	
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
} 