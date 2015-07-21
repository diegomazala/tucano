#version 330 core

// Interpolated values from the vertex shaders
in vec3 normalCoord;
in vec2 texCoord;
in vec3 positionWorldSpace;

in vec3 lightDirectionTangentSpace;
in vec3 eyeDirectionTangentSpace;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D in_Texture_Diffuse;
uniform sampler2D in_Texture_Normal;
uniform sampler2D in_Texture_Specular;
uniform sampler2D in_Texture_Height;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 MV3x3;
uniform mat4 lightViewMatrix;
uniform float lightIntensity;

uniform bool diffuseTextureEnabled = true;
uniform bool normalTextureEnabled = false;
uniform bool specularTextureEnabled = true;
uniform bool parallaxTextureEnabled = true;
uniform float parallaxScale = 0.05f;
uniform float parallaxBias = -0.04f;



void main()
{
	vec2 newTexCoord = texCoord;

	// Light emission properties
	vec3 lightColor = vec3(1, 1, 1);


	if (parallaxTextureEnabled)
	{
		float height = texture2D(in_Texture_Height, texCoord).r;

		vec3 v = normalize(eyeDirectionTangentSpace);
		height = height * parallaxScale + parallaxBias;
		newTexCoord = texCoord + (height * v.xy);
	}
	

	// Material properties
	vec3 diffuseColor = vec3(0.5f, 0.5f, 0.5f);
	if (diffuseTextureEnabled)
		diffuseColor = texture2D(in_Texture_Diffuse, newTexCoord).rgb;

	vec3 ambientColor = vec3(0.1, 0.1, 0.1) * diffuseColor;

	vec3 specularColor = vec3(0.5f, 0.5f, 0.5f);
	if (specularTextureEnabled)
		specularColor = texture2D(in_Texture_Specular, newTexCoord).rgb * 0.3;

	//vec3 texNormalTangentSpace = normalize(texture2D(in_Texture_Normal, texCoord).rgb*2.0 - 1.0);
	vec3 texNormalTangentSpace = normalize(texture2D(in_Texture_Normal, newTexCoord).rgb*2.0 - 1.0);

	// Normal of the computed fragment, in camera space
	//vec3 n = texNormalTangentSpace;
	vec3 n = normalCoord;
	if (normalTextureEnabled)
		n = texNormalTangentSpace;
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(lightDirectionTangentSpace);
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp(dot(n, l), 0, 1);

	// Eye vector (towards the camera)
	vec3 E = normalize(eyeDirectionTangentSpace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l, n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp(dot(E, R), 0, 1);

	color =
		ambientColor +
		diffuseColor * lightColor * lightIntensity * cosTheta +
		specularColor * lightColor * lightIntensity * pow(cosAlpha, 5);
}