#version 430 core


in vec4 in_Position;

out vec3 vPosition;


void main()
{
	vPosition = in_Position.xyz;
}