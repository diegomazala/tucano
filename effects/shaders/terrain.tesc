#version 430 core



layout (vertices = 4) out;


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
	float depth;
} tc_out[]; 


uniform float MinTessLevel;
uniform float MaxTessLevel;
uniform float MaxDepth;
uniform float MinDepth;

uniform float tessLevelInner;
uniform float tessLevelOuter;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

void main()
{

	// position in camera coordinates
	vec4 p = modelViewMatrix * gl_in[gl_InvocationID].gl_Position;

	// distance from camera between 0 and 1
	float depth = clamp(( abs(p.z) - MinDepth) / (MaxDepth - MinDepth), 0.0, 1.0);

	// interpolate between min/max tess levels 
	float tessLevel = mix(MaxTessLevel, MinTessLevel, depth);

    if (gl_InvocationID == 0)
	{
        //gl_TessLevelInner[0] = tessLevelInner;
        //gl_TessLevelOuter[0] = tessLevelOuter;
        //gl_TessLevelOuter[1] = tessLevelOuter;
		//gl_TessLevelOuter[2] = tessLevelOuter;
		
		gl_TessLevelInner[0] = tessLevel;
		gl_TessLevelInner[1] = tessLevel;
		gl_TessLevelOuter[0] = tessLevel;
		gl_TessLevelOuter[1] = tessLevel;
		gl_TessLevelOuter[2] = tessLevel;
		gl_TessLevelOuter[3] = tessLevel;
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	
	tc_out[gl_InvocationID].position = tc_in[gl_InvocationID].position;
	tc_out[gl_InvocationID].normal = tc_in[gl_InvocationID].normal;
	tc_out[gl_InvocationID].uv = tc_in[gl_InvocationID].uv;
	tc_out[gl_InvocationID].depth = depth;

}

