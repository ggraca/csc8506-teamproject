#version 400 core

uniform sampler2D depthTex;
uniform sampler2D normTex;
uniform sampler2DShadow shadowTex;

uniform vec2 pixelSize;
uniform vec3 cameraPos;
in mat4 inverseProjView;
uniform mat4 shadowMatrix;
uniform bool drawShadows;

uniform float lightBrightness;
uniform vec3 lightDir;
uniform vec4 lightColour;

out vec4 fragColour [2];

void main (void) {	
	vec3 pos = vec3((gl_FragCoord.x * pixelSize.x ), (gl_FragCoord.y * pixelSize.y), 0.0);
	pos.z = texture(depthTex, pos.xy ).r;

	vec3 normal = normalize(texture(normTex, pos.xy).xyz * 2.0 - 1.0);

	vec4 clip = inverseProjView * vec4(pos * 2.0 - 1.0, 1.0);

	if(pos.z == 1.0) {
		discard;
	}
	
	pos = clip.xyz / clip.w;
	
	vec4 shadowProj = (shadowMatrix * vec4(pos + (normal * 1.5), 1));
	float shadow = 1.0; // New !

	if(shadowProj.w > 0.0) { // New !
		shadow = textureProj(shadowTex, shadowProj);
	}

	vec3 incident = lightDir;
	vec3 viewDir = normalize(cameraPos - pos);
	vec3 halfDir = normalize(incident + viewDir);

	float lambert = clamp(dot(incident, normal), 0.0, 1.0);
	float rFactor = clamp(dot(halfDir , normal), 0.0, 1.0);
	float sFactor = pow(rFactor, 33.0);

	vec4 finalCol = vec4(lightColour.xyz * lambert, 1.0) * lightBrightness;
	vec4 specularCol = vec4(lightColour.xyz * sFactor, 1.0);
	
	if (drawShadows){
		finalCol = finalCol * shadow;
		specularCol = specularCol * shadow;
	}
	
	fragColour [0] = finalCol;
	fragColour [1] = specularCol;
}