#version 400

layout(triangles, cw) in;

uniform sampler2D heightMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in Vertex	{ 	//Sent from the TCS
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent; // New !
	vec3 binormal; // New !
	vec3 worldPos;
} IN[];			//Equal to TCS layout size

out Vertex	{ 	//Each TES works on a single vertex!
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent; // New !
	vec3 binormal; // New !
	vec3 worldPos;
} OUT;

vec4	QuadMixVec4(vec4 a, vec4 b, vec4 c, vec4 d) {
	vec4 p0 		 = mix(a,c, gl_TessCoord.x);
	vec4 p1 		 = mix(b,d, gl_TessCoord.x);
	
	return mix(p0,p1,gl_TessCoord.y);
}

vec3	QuadMixVec3(vec3 a, vec3 b, vec3 c, vec3 d) {
	vec3 p0 		 = mix(a,c, gl_TessCoord.x);
	vec3 p1 		 = mix(b,d, gl_TessCoord.x);
	
	return mix(p0,p1,gl_TessCoord.y);
}

vec2	QuadMixVec2(vec2 a, vec2 b, vec2 c, vec2 d) {
	vec2 p0 		 = mix(a,c, gl_TessCoord.x);
	vec2 p1 		 = mix(b,d, gl_TessCoord.x);
	
	return mix(p0,p1,gl_TessCoord.y);
}

vec4 TriangleMixVec4(vec4 a, vec4 b, vec4 c){
	vec4 accum = vec4(0.0f);
	
	accum += a * gl_TessCoord[0];
	accum += b * gl_TessCoord[1];
	accum += c * gl_TessCoord[2];
	
	return accum;
}

vec3 TriangleMixVec3(vec3 a, vec3 b, vec3 c){
	vec3 accum = vec3(0.0f);
	
	accum += a * gl_TessCoord[0];
	accum += b * gl_TessCoord[1];
	accum += c * gl_TessCoord[2];
	
	return accum;
}

vec2 TriangleMixVec2(vec2 a, vec2 b, vec2 c){
	vec2 accum = vec2(0.0f);
	
	accum += a * gl_TessCoord[0];
	accum += b * gl_TessCoord[1];
	accum += c * gl_TessCoord[2];
	
	return accum;
}

void main()	{			 		
	vec3 combinedPos = TriangleMixVec3(	gl_in[0].gl_Position.xyz ,
												gl_in[1].gl_Position.xyz ,
												gl_in[2].gl_Position.xyz);
												
	OUT.colour = TriangleMixVec4(IN[0].colour, 
										IN[1].colour,
										IN[2].colour);

	vec2 texCoord = TriangleMixVec2(IN[0].texCoord, 
										IN[1].texCoord,
										IN[2].texCoord);
	
	vec3 normal = TriangleMixVec3(IN[0].normal, 
										IN[1].normal,
										IN[2].normal);
										
	OUT.tangent = TriangleMixVec3(IN[0].tangent, 
										IN[1].tangent,
										IN[2].tangent);
										
	OUT.binormal = TriangleMixVec3(IN[0].binormal, 
										IN[1].binormal,
										IN[2].binormal);
										
	
	float height = texture(heightMap, vec2(texCoord.x, (1 - texCoord.y))).x;
	
	combinedPos += normal * height;
	OUT.texCoord = texCoord;
	OUT.normal = normal;
	OUT.worldPos = combinedPos;
		
	gl_Position = (projMatrix * viewMatrix) * vec4(combinedPos, 1.0f);
}