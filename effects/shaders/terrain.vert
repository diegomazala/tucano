#version 430 core


in vec4 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoords;
in vec4 in_Color;




uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;


out VS_OUT
{
    vec3 position;
	vec3 normal;
	vec2 uv;
} vs_out;   


void main(void)
{
	//normal = normalize(normalMatrix * normalize(in_Normal));
	//vert = modelViewMatrix * in_Position;
	//texCoords = in_TexCoords;

	vs_out.position = (modelViewMatrix * in_Position).xyz;
	//vs_out.normal	= normalize(normalMatrix * normalize(in_Normal));
	vs_out.normal	= in_Normal;
	vs_out.uv		= in_TexCoords;

	gl_Position = in_Position;
	//gl_Position = (projectionMatrix * modelViewMatrix) * in_Position;
}

