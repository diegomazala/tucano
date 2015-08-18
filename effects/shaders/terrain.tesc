#version 430 core



layout (vertices = 3) out;


in VS_OUT
{
    vec3 position;
	vec3 normal;
	vec2 uv;
} tc_in[]; 


out TC_OUT
{
    vec3 position;
	vec3 normal;
	vec2 uv;
} tc_out[]; 



uniform float tessLevelInner;
uniform float tessLevelOuter;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

void main()
{
    if (gl_InvocationID == 0)
	{
        gl_TessLevelInner[0] = tessLevelInner;
        gl_TessLevelOuter[0] = tessLevelOuter;
        gl_TessLevelOuter[1] = tessLevelOuter;
		gl_TessLevelOuter[2] = tessLevelOuter;
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	
	tc_out[gl_InvocationID].position = tc_in[gl_InvocationID].position;
	tc_out[gl_InvocationID].normal = tc_in[gl_InvocationID].normal;
	tc_out[gl_InvocationID].uv = tc_in[gl_InvocationID].uv;
}

