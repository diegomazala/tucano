
#version 430 core


in TE_OUT
{
	vec3 position;
	vec3 normal;
	vec2 uv;
	float depth;
} fs_in;


uniform sampler2D in_HeightMap;

uniform mat4 lightViewMatrix;
uniform bool lightEnabled;

bool showColorGrade = true;

out vec4 outColor;

void main(void)
{
	if (showColorGrade)
	{
		if (fs_in.depth < 0.2)
			outColor = vec4(0.5, 1, 1, 1);
		else if (fs_in.depth < 0.4)
			outColor = vec4(0.5, 0.5, 1, 1);
		else if (fs_in.depth < 0.6)
			outColor = vec4(1, 0.5, 0.5, 1);
		else if (fs_in.depth < 0.8)
			outColor = vec4(1, 1, 0.5, 1);
		else
			outColor = vec4(1, 0.5, 1, 1);
	}
	else
	{
		outColor = vec4(1);
	}
}



