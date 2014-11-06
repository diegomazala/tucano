#version 430
	 
in vec4 vert;
in vec3 normal;

out vec4 out_Coords;
out vec4 out_Normal;

uniform float camera_translation;
	 
void main(void)
{
//    float depth = vert[2];
//    depth = (depth+camera_translation)/2.0;//camera_translation is because of the normalization translation applied to view matrix.

    //out_Color = vec4(depth, 0.0, 0.0, 1.0);

    out_Coords = vec4 (vert.xyz, 1.0);
    out_Normal = vec4 (normal.xyz, 1.0);
}
