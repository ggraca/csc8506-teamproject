#version 400

layout(quads, cw) in;

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

void main()	{			 		
	vec3 combinedPos = QuadMixVec3(	gl_in[0].gl_Position.xyz ,
												gl_in[1].gl_Position.xyz ,
												gl_in[2].gl_Position.xyz ,
												gl_in[3].gl_Position.xyz	);
												
	OUT.colour = QuadMixVec4(IN[0].colour, 
										IN[1].colour,
										IN[2].colour,
										IN[3].colour	);

	OUT.texCoord = QuadMixVec2(IN[0].texCoord, 
										IN[1].texCoord,
										IN[2].texCoord,
										IN[3].texCoord	);
	
	OUT.normal = QuadMixVec3(IN[0].normal, 
										IN[1].normal,
										IN[2].normal,
										IN[3].normal	);
										
	OUT.tangent = QuadMixVec3(IN[0].tangent, 
										IN[1].tangent,
										IN[2].tangent,
										IN[3].tangent	);
										
	OUT.binormal = QuadMixVec3(IN[0].binormal, 
										IN[1].binormal,
										IN[2].binormal,
										IN[3].binormal	);
										
	OUT.worldPos = QuadMixVec3(IN[0].worldPos, 
										IN[1].worldPos,
										IN[2].worldPos,
										IN[3].worldPos	);
	
	float height = texture(heightMap, OUT.texCoord  ).x;
	
	combinedPos.y += height * 500;
	OUT.worldPos.y += height * 500;
		
	gl_Position = vec4(combinedPos, 1.0f);
}