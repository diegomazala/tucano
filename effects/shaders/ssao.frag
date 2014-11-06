#version 430
#define numberOfSamples 16

in vec4 ex_Color;
in vec3 normalDirection;
in vec4 vert;

out vec4 out_Color;

uniform mat4 lightViewMatrix;

uniform vec2 noiseScale;
uniform float radius;
uniform vec3 kernel[numberOfSamples];
uniform sampler2D noiseTexture;
uniform sampler2D depthTexture;
uniform mat4 projectionMatrix;
uniform bool displayAmbientPass;
uniform ivec2 viewportSize;

uniform vec2 randomTest;

float computeOcclusion(void)
{

    //origin is the fragment's position in view space:
    vec3 origin = vert.xyz;

    //normal is in view space.
    vec3 normal = normalDirection;
    normal = normalize(normal);

    //Retrieve texCoord for random vector generation.
    vec2 texCoord = vec2(gl_FragCoord.x/float(viewportSize[0]), gl_FragCoord.y/float(viewportSize[1]));
    texCoord *= noiseScale;

    //tbn will reorient the kernel to the normal's direction, as well as apply a random rotation to reduce banding.
    vec3 randomVector = normalize(texture(noiseTexture, texCoord).xyz * 2.0 - 1.0);
    vec3 tangent = normalize(randomVector - normal * dot(randomVector, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);

    //Loop through the sample kernel, sampling the depth buffer and accumulating the occlusion:
    float occlusion = 0.0;
    for(int i = 0; i < numberOfSamples; i++)
    {

        //samplePosition is in view space.
        vec3 samplePosition = tbn * kernel[i];
        samplePosition = samplePosition * radius + origin;

        //Offset is the projection of samplePosition in screen space, that will be used in order to sample the depth texture.
        vec4 offset = vec4(samplePosition,1.0);
        offset = projectionMatrix * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        //sampleDepth is the actual depth of the sample point in view space.
        float sampleDepth = texture(depthTexture,offset.xy).r;

        //sampleDepth = (1 - sampleDepth) * -1.0;
        sampleDepth = sampleDepth * 2.0 - 3.0;

        //range check & accumulate:
        float range_check = abs(origin.z - sampleDepth) < radius ? 1.0 : 0.0;

        //This test compares wether the sampleDepth is in front of samplePosition. If it is, then samplePosition is inside geometry and contributes to occlusion.
        occlusion += (sampleDepth > samplePosition.z ? 1.0 : 0.0) * range_check;

    }

    occlusion = 1 - (occlusion / float(numberOfSamples));

    return occlusion;

}

void main(void)
{

    float occlusion = computeOcclusion();

    vec3 lightDirection = (lightViewMatrix * vec4(0.0, 0.0, 1.0, 0.0)).xyz;
    lightDirection = normalize(lightDirection);

    vec3 lightReflection = reflect(-lightDirection, normalDirection);
    vec3 eyeDirection = -normalize(vert.xyz);
    float shininess = 100.0;

    vec4 ambientLight = vec4(1.0);

    if(displayAmbientPass)
    {
        ambientLight *= occlusion;
    }
    else
    {
        ambientLight *= ex_Color * occlusion * 0.5;
    }

    vec4 diffuseLight = ex_Color * 0.4 * max(dot(lightDirection, normalDirection),0.0);
    vec4 specularLight = vec4(1.0) *  max(pow(dot(lightReflection,eyeDirection),shininess),0.0);

    if(displayAmbientPass)
    {
        out_Color = vec4(ambientLight.xyz, 1.0);
    }
    else
    {
        out_Color = vec4(ambientLight.xyz + diffuseLight.xyz + specularLight.xyz, 1.0);
    }

}
