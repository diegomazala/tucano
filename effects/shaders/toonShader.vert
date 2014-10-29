#version 420
	 
layout(location=0) in vec4 in_Position;
layout(location=1) in vec3 in_Normal;
//layout(location=2) in vec4 in_Color;

out vec4 ex_Color;
out vec3 normalDirection;
out vec4 vert;
//out mat3 lightRotationMatrix;
	 
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
//uniform mat3 lightViewMatrix;
uniform vec4 in_Color;
	 
void main(void)
{
	mat4 modelViewMatrix = viewMatrix*modelMatrix;
	mat4 normalMatrix = transpose(inverse(modelViewMatrix));

	normalDirection = normalize(vec3(normalMatrix * vec4(in_Normal,0.0)).xyz);
	vert = modelViewMatrix*in_Position;

    gl_Position = (projectionMatrix * modelViewMatrix) * in_Position;
	ex_Color = in_Color;
	//ex_Color = vec4(in_Normal.xyz,1.0);
}
