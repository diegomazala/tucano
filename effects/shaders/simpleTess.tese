#version 430 core


layout(triangles, equal_spacing, cw) in;
in vec3 tcPosition[];
out vec3 tePosition;
out vec3 tePatchDistance;

in vec3 tcNormal[];
out vec3 teNormal;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

void main()
{
    vec3 p0 = gl_TessCoord.x * tcPosition[0];
    vec3 p1 = gl_TessCoord.y * tcPosition[1];
    vec3 p2 = gl_TessCoord.z * tcPosition[2];
    tePatchDistance = gl_TessCoord;
    tePosition = normalize(p0 + p1 + p2);

	gl_Position = projectionMatrix * modelViewMatrix * vec4(tePosition, 1);

	teNormal = tcNormal[0];
}
