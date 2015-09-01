
#version 430 core

layout (quads, fractional_even_spacing, cw) in;




in TC_OUT
{
    vec3 position;
	vec3 normal;
	vec2 uv;
	float depth;
} te_in[]; 


out TE_OUT
{
    vec3 position;
	vec3 normal;
	vec2 uv;
	float depth;
} te_out; 


uniform sampler2D in_HeightMap;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform float depthLevel;


///////////////////////////////////////////////////////////
// Interpolation for triangles
//
float interpolate(in float v0, in float v1, in float v2)
{
	return float((gl_TessCoord.x * v0) +
				(gl_TessCoord.y * v1) +
				(gl_TessCoord.z * v2));

}
vec2 interpolate(in vec2 v0, in vec2 v1, in vec2 v2)
{
	return vec2((gl_TessCoord.x * v0) +
				(gl_TessCoord.y * v1) +
				(gl_TessCoord.z * v2));

}
vec3 interpolate(in vec3 v0, in vec3 v1, in vec3 v2)
{
	return vec3((gl_TessCoord.x * v0) +
				(gl_TessCoord.y * v1) +
				(gl_TessCoord.z * v2));

}
vec4 interpolate(in vec4 v0, in vec4 v1, in vec4 v2)
{
	return vec4((gl_TessCoord.x * v0) +
				(gl_TessCoord.y * v1) +
				(gl_TessCoord.z * v2));
}
// Interpolation for triangles
//////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
// Interpolation for quads
//
float interpolate(in float v0, in float v1, in float v2, in float v3)
{
	float a = mix(v0, v1, gl_TessCoord.x);
	float b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}
vec2 interpolate(in vec2 v0, in vec2 v1, in vec2 v2, in vec2 v3)
{
	vec2 a = mix(v0, v1, gl_TessCoord.x);
	vec2 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}
vec3 interpolate(in vec3 v0, in vec3 v1, in vec3 v2, in vec3 v3)
{
	vec3 a = mix(v0, v1, gl_TessCoord.x);
	vec3 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}
vec4 interpolate(in vec4 v0, in vec4 v1, in vec4 v2, in vec4 v3)
{
	vec4 a = mix(v0, v1, gl_TessCoord.x);
	vec4 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}
// Interpolation for quads
///////////////////////////////////////////////////////////

void main()
{


	vec4 tePos = interpolate(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position);
	te_out.position = (modelViewMatrix * tePos).xyz;

	vec3 teNormal =	interpolate(te_in[0].normal, te_in[1].normal, te_in[2].normal, te_in[3].normal);
	te_out.normal = teNormal;
	te_out.normal = vec3(modelViewMatrix * vec4(teNormal,0.0)).xyz;

	vec2 teUV =	interpolate(te_in[0].uv, te_in[1].uv, te_in[2].uv, te_in[3].uv);
	te_out.uv = teUV;

	te_out.depth =	interpolate(te_in[0].depth, te_in[1].depth, te_in[2].depth, te_in[3].depth);

	float height = texture(in_HeightMap, te_out.uv).r;
	tePos.y += height * depthLevel - (depthLevel * 0.5f);

	gl_Position = projectionMatrix * modelViewMatrix * tePos;
	te_out.position = (modelViewMatrix * tePos).xyz;
}

