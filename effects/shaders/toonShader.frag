#version 420
	 
in vec4 ex_Color;
in vec3 normalDirection;
in vec4 vert;
in mat3 lightRotationMatrix;

out vec4 out_Color;

uniform mat3 lightViewMatrix;
uniform float quantizationLevel;
	 
void main(void)
{

	vec3 currentColor;

	vec3 lightDirection = lightViewMatrix * vec3(0.0, 0.0, 1.0);
	lightDirection = normalize(lightDirection);
	
	vec3 lightReflection = reflect(-lightDirection, normalDirection);
	vec3 eyeDirection = -normalize(vert.xyz);
	float shininess = 100.0;

	vec4 diffuseLight = ex_Color * max(dot(lightDirection, normalDirection),0.0);
	vec4 specularLight = vec4(1.0) *  max(pow(dot(lightReflection,eyeDirection),shininess),0.0);

	float intensity = dot(lightDirection,normalDirection);

	currentColor = vec3(diffuseLight.xyz + specularLight.xyz);

	//Begin color quantization for toon effect:

	currentColor = floor(0.5 + (quantizationLevel * currentColor)) / quantizationLevel;

	out_Color = vec4(currentColor,1.0);

}


