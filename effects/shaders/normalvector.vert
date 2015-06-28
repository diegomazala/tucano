#version 430
layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec3 in_Normal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * in_Position;
	
	mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
	vs_out.normal = normalize(vec3(projectionMatrix * vec4(normalMatrix * in_Normal, 1.0)));
}