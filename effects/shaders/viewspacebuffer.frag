#version 430
	 
in vec4 vert;
in vec3 normal;

out vec4 out_Coords;
out vec4 out_Normal;

uniform float camera_translation;
	 
void main(void)
{
    out_Coords = vec4 (vert.xyz, 1.0);
    out_Normal = vec4 (normal.xyz, 1.0);
}
