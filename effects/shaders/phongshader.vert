#version 420

in vec4 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoords;
in vec4 in_Color;

out vec4 ex_Color;
out vec3 normalDirection;
out vec4 vert;
out vec2 texCoords;

out float depth;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


// if attribute in_Color exists or not
uniform bool has_color;

const vec4 default_color = vec4(0.5, 0.5, 0.9, 1.0);

void main(void)
{
	mat4 modelViewMatrix = viewMatrix * modelMatrix;

	mat4 normalMatrix = transpose(inverse(modelViewMatrix));
	normalDirection = normalize(vec3(normalMatrix * vec4(in_Normal,0.0)).xyz);

	vert = modelViewMatrix * in_Position;

	depth = in_Position.z;

	texCoords = in_TexCoords;

	gl_Position = (projectionMatrix * modelViewMatrix) * in_Position;

        if (has_color)
            ex_Color = in_Color;
        else
            ex_Color = default_color;

	//ex_Color = vec4(in_Normal.xyz,1.0);
}
