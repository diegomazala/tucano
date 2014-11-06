#version 430
#define numberOfSamples 16

in vec4 ex_Color;

out vec4 out_Color;

uniform mat4 lightViewMatrix;

uniform vec2 noiseScale;
uniform vec3 kernel[numberOfSamples];
uniform sampler2D depthTexture;
uniform sampler2D normalTexture;
uniform bool displayAmbientPass;

uniform float radius;
uniform sampler2D noiseTexture;
uniform mat4 projectionMatrix;
uniform ivec2 viewportSize;

/*
float computeOcclusion(void)
{

    // origin is the vertex position in view space:
    vec3 origin = vert.xyz;

    // normal is in view space.
    vec3 normal = normalize(normalDirection);

    // retrieve texCoord for random vector generation.
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
        sampleDepth = sampleDepth * 2.0 - 4.0;

        //range check & accumulate:
        float range_check = abs(origin.z - sampleDepth) < radius ? 1.0 : 0.0;

        //This test compares wether the sampleDepth is in front of samplePosition. If it is, then samplePosition is inside geometry and contributes to occlusion.
        occlusion += (sampleDepth > samplePosition.z ? 1.0 : 0.0) * range_check;

    }

    occlusion = 1 - (occlusion / float(numberOfSamples));

    return occlusion;

}
*/

float ambientOcclusion (vec3 vert, vec3 normal)
{
    vec2 texCoord = gl_FragCoord.xy;
    float occlusion = 0.0;

    for(int i = 0; i < numberOfSamples; i++)
    {
        vec3 samplePosition = kernel[i];
        vec2 randCoord = texCoord + samplePosition.xy * 100;

        randCoord /= vec2(viewportSize.xy);
        vec4 point = texture(depthTexture, randCoord);

        if (point != vec4(0.0))
        {
            vec3 v = point.xyz - vert.xyz;
            float dist_factor = 1.0 / (1.0 + length(v));
            occlusion += max(0.0, dot (normal, v)) * dist_factor;
        }
    }

    occlusion = 1 - (occlusion / float(numberOfSamples));
    return occlusion;
}

void main (void)
{
    vec4 vert = texelFetch(depthTexture, ivec2(gl_FragCoord.xy), 0);
    if (vert.w == 0.0)
        discard;

    vec3 normal = texelFetch(normalTexture, ivec2(gl_FragCoord.xy), 0).xyz;

    //float occlusion = computeOcclusion();
    float occlusion = ambientOcclusion(vert.xyz, normal);

    vec3 lightDirection = (lightViewMatrix * vec4(0.0, 0.0, 1.0, 0.0)).xyz;
    lightDirection = normalize(lightDirection);

    vec3 lightReflection = reflect(-lightDirection, normal);
    vec3 eyeDirection = -normalize(vert.xyz);
    float shininess = 100.0;

    vec4 ambientLight = vec4(0.5);

    vec4 color = vec4(0.7, 0.7, 0.7, 1.0);

    if(!displayAmbientPass)
    {
        ambientLight = vec4(1.0) * occlusion;
    }
    else
    {
        ambientLight *= color;// * occlusion;
    }

    vec4 diffuseLight = color * 0.4 * max(dot(lightDirection, normal),0.0);
    vec4 specularLight = vec4(1.0) *  max(pow(dot(lightReflection,eyeDirection),shininess),0.0);

    if(!displayAmbientPass)
    {
        out_Color = vec4(ambientLight.xyz, 1.0);
    }
    else
    {
        out_Color = occlusion*vec4(ambientLight.xyz + diffuseLight.xyz + specularLight.xyz, 1.0);
    }
}
