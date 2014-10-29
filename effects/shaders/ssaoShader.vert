#version 430
	 
in vec4 in_Position;
in vec3 in_Normal;
//layout(location=2) in vec4 in_Color;
in vec2 in_TexCoords;
//layout(binding=0) uniform sampler2D noiseTexture;//Segmentation fault when reloading shaders. Why?
uniform sampler2D noiseTexture;


out vec4 ex_Color;
out vec3 normalDirection;
out vec4 vert;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec4 in_Color;
	 
void main(void)
{
    mat4 modelViewMatrix = viewMatrix*modelMatrix;
    mat4 normalMatrix = transpose(inverse(modelViewMatrix));

    normalDirection = normalize(vec3(normalMatrix * vec4(in_Normal,0.0)).xyz);
    vert = modelViewMatrix*in_Position;

    gl_Position = (projectionMatrix * modelViewMatrix) * in_Position;
    ex_Color = in_Color;

}
