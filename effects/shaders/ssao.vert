#version 430
	 
in vec4 in_Position;

out vec4 ex_Color;
	 
void main(void)
{
    gl_Position = in_Position;
}
