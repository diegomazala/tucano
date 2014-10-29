#version 430

in vec4 in_Position;

out vec4 vert;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

out float translation;

void main(void)
{
    mat4 modelViewMatrix = viewMatrix*modelMatrix;
    vert = modelViewMatrix * in_Position;
    gl_Position = (projectionMatrix * modelViewMatrix) * in_Position;
}
