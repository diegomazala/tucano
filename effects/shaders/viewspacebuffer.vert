#version 430

in vec4 in_Position;
in vec3 in_Normal;

out vec4 vert;
out vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

out float translation;

void main(void)
{
    mat4 modelViewMatrix = viewMatrix*modelMatrix;
    mat4 normalMatrix = transpose(inverse(modelViewMatrix));
    normal = normalize(vec3(normalMatrix * vec4(in_Normal, 0.0)).xyz);
    vert = modelViewMatrix * in_Position;

    gl_Position = (projectionMatrix * modelViewMatrix) * in_Position;
}
