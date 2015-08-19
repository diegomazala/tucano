
#version 430 core


in GS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 uv;
	vec3 triDistance;
	float depth;
} fs_in;


uniform sampler2D in_HeightMap;

uniform mat4 lightViewMatrix;
uniform bool lightEnabled;
uniform bool wireframeEnabled;

bool showColorGrade = true;


out vec4 outColor;


float amplify(float d, float scale, float offset)
{
	d = scale * d + offset;
	d = clamp(d, 0, 1);
	d = 1 - exp2(-2 * d*d);
	return d;
}


void main(void)
{
	vec3 diffuse = vec3(0.5);

	if (showColorGrade)
	{
		if (fs_in.depth < 0.2)
			diffuse = vec3(0.5, 1, 1);
		else if (fs_in.depth < 0.4)
			diffuse = vec3(0.5, 0.5, 1);
		else if (fs_in.depth < 0.6)
			diffuse = vec3(1, 0.5, 0.5);
		else if (fs_in.depth < 0.8)
			diffuse = vec3(1, 1, 0.5);
		else
			diffuse = vec3(1, 0.5, 1);
	}

	float edge = 1;
	
	if (wireframeEnabled)
	{
		float d1 = min(min(fs_in.triDistance.x, fs_in.triDistance.y), fs_in.triDistance.z);
		edge = amplify(d1, 40, -0.5);
		//edge = amplify(d1, 100 * (1.0 - fs_in.depth), -0.5);
	}


	vec3 lightDirection = (lightViewMatrix * vec4(0.0, 0.0, 1.0, 0.0)).xyz;
	lightDirection = normalize(lightDirection);

	vec3 N = normalize(fs_in.normal);
	vec3 L = lightDirection;
	float df = abs(dot(N, L));
	vec3 color = diffuse * df * edge;

	outColor = vec4(color, 1);

}



