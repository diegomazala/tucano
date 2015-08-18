
#version 430 core


in TE_OUT
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} fs_in;


uniform sampler2D in_HeightMap;

uniform mat4 lightViewMatrix;
uniform bool lightEnabled;

out vec4 outColor;

void main(void)
{
	outColor = vec4(1);
}



