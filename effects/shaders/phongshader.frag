#version 430

in vec4 color;
in vec3 normal;
in vec4 vert;

in vec2 texCoords;
in float depth;

out vec4 out_Color;

uniform mat4 lightViewMatrix;

void main(void)
{
    vec3 lightDirection = (lightViewMatrix * vec4(0.0, 0.0, 1.0, 0.0)).xyz;
    lightDirection = normalize(lightDirection);

    vec3 lightReflection = reflect(-lightDirection, normal);
    vec3 eyeDirection = -normalize(vert.xyz);
    float shininess = 100.0;

    vec4 ambientLight = color * 0.5;
    vec4 diffuseLight = color * 0.4 * max(dot(lightDirection, normal),0.0);
    vec4 specularLight = vec4(1.0) *  max(pow(dot(lightReflection, eyeDirection), shininess),0.0);

    out_Color = vec4(ambientLight.xyz + diffuseLight.xyz + specularLight.xyz,1.0);

}
