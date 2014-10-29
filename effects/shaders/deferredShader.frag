#version 430
	 
in vec4 vert;

out vec4 out_Color;
	 
void main(void)
{
    float depth = vert[2];
    depth = (depth+3)/2.0;//+3 is because of the normalization translation applied to view matrix.

    out_Color = vec4(depth,0.0,0.0,1.0);
}
