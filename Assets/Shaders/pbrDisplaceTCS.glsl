#version 400 core
layout(vertices =4) out;	//num vertices in output patch
 
in Vertex	{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent; // New !
	vec3 binormal; // New !
	vec3 worldPos;
} IN[];				//Equal to size of the draw call vertex count

out Vertex	{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent; // New !
	vec3 binormal; // New !
	vec3 worldPos;	//To Evaluation Shader
} OUT[];				//Equal to the size of the layout vertex count

void main()	{
	gl_TessLevelInner[0] = 32;
	gl_TessLevelInner[1] = 32;
	gl_TessLevelOuter[0] = 32;
	gl_TessLevelOuter[1] = 32;
	gl_TessLevelOuter[2] = 32;
	gl_TessLevelOuter[3] = 32;

	OUT[gl_InvocationID].colour   = IN[gl_InvocationID].colour;
	OUT[gl_InvocationID].texCoord = IN[gl_InvocationID].texCoord;
	OUT[gl_InvocationID].normal   = IN[gl_InvocationID].normal;
	OUT[gl_InvocationID].tangent  = IN[gl_InvocationID].tangent; // New !
	OUT[gl_InvocationID].binormal = IN[gl_InvocationID].binormal; // New !
	OUT[gl_InvocationID].worldPos = IN[gl_InvocationID].worldPos;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}