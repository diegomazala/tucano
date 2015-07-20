#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_Tangent;
layout(location = 4) in vec3 in_Bitangent;

// Output data ; will be interpolated for each fragment.
out vec2 texCoord;
out vec3 positionWorldSpace;
out vec3 eyeDirectionCameraSpace;
out vec3 lightDirectionCameraSpace;

out vec3 lightDirectionTangentSpace;
out vec3 eyeDirectionTangentSpace;

// Values that stay constant for the whole mesh.
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 MV3x3;
uniform mat4 lightViewMatrix;

void main()
{
	mat4 modelViewMatrix = viewMatrix * modelMatrix;

	// Output position of the vertex, in clip space : MVP * position
	gl_Position = (projectionMatrix * modelViewMatrix) * vec4(in_Position, 1);

	// Position of the vertex, in worldspace : modelMatrix * position
	positionWorldSpace = (modelMatrix * vec4(in_Position, 1)).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPositionCameraSpace = (viewMatrix * modelMatrix * vec4(in_Position, 1)).xyz;
	eyeDirectionCameraSpace = vec3(0, 0, 0) - vertexPositionCameraSpace;

	// Compute light direction
	vec3 lightDirection = (lightViewMatrix * vec4(0.0, 0.0, 1.0, 0.0)).xyz;
	lightDirectionCameraSpace = normalize(lightDirection);

	// UV of the vertex. No special space for this one.
	texCoord = in_TexCoord;

	// model to camera = ModelView
	vec3 vertexTangentCameraSpace = MV3x3 * in_Tangent;
	vec3 vertexBitangentCameraSpace = MV3x3 * in_Bitangent;
	vec3 vertexNormalCameraSpace = MV3x3 * in_Normal;

	mat3 TBN = transpose(mat3(
		vertexTangentCameraSpace,
		vertexBitangentCameraSpace,
		vertexNormalCameraSpace
		)); // You can use dot products instead of building this matrix and transposing it. See References for details.

	lightDirectionTangentSpace = TBN * lightDirectionCameraSpace;
	eyeDirectionTangentSpace = TBN * eyeDirectionCameraSpace;
}

