#version 430

in vec4 in_Position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main (void)
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * in_Position;
    //gl_Position = in_Position;

}
