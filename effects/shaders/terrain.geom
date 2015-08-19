#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in TE_OUT
{
	vec3 position;
	vec3 normal;
	vec2 uv;
	float depth;
} gs_in[];


uniform mat3 normalMatrix;


out GS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 uv;
	vec3 triDistance;
	float depth;
} gs_out;


void main()
{
	gs_out.depth = (gs_in[0].depth + gs_in[1].depth + gs_in[2].depth) / 3.0f;

	vec3 A = gs_in[2].position - gs_in[0].position;
	vec3 B = gs_in[1].position - gs_in[0].position;
	gs_out.normal = normalMatrix * normalize(cross(A, B));

	gs_out.triDistance = vec3(1, 0, 0);
	gl_Position = gl_in[0].gl_Position; 
	EmitVertex();

	gs_out.triDistance = vec3(0, 1, 0);
	gl_Position = gl_in[1].gl_Position; 
	EmitVertex();

	gs_out.triDistance = vec3(0, 0, 1);
	gl_Position = gl_in[2].gl_Position; 
	EmitVertex();

	EndPrimitive();
}
