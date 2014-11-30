#version 430
#define numberOfSamples 64

out vec4 out_Color;

uniform mat4 lightViewMatrix;

uniform vec2 noiseScale;
uniform vec2 kernel[numberOfSamples];
uniform sampler2D coordsTexture;
uniform sampler2D normalTexture;

uniform float radius;
uniform float intensity;
uniform float max_dist;

uniform sampler2D noiseTexture;

float ambientOcclusion (vec3 vert, vec3 normal)
{
    ivec2 texCoord = ivec2(gl_FragCoord.xy);
    float occlusion = 0.0;

    vec2 noisecoords = gl_FragCoord.xy;// / vec2(textureSize(noiseTexture, 0).xy);
    float noise = texelFetch(noiseTexture, ivec2(noisecoords).xy, 0).x;

    noise *= 3.14;
    mat2 rotation = mat2(cos(noise), -sin(noise), sin(noise), cos(noise));

    float depth = abs(vert.z);
    float max_v = 35.0;
    // smaller radius for points farther away
    int rad = max(5, min(int(max_v), int(max_v/depth)));
    for (int i = 0; i <= numberOfSamples; ++i)
    {
        vec2 rotated = rotation * kernel[i].xy;
        // rotate by random vector to avoid unwanted patterns
        ivec2 randCoord = texCoord + ivec2(rotated * rad);

        vec4 point = texelFetch(coordsTexture, randCoord, 0);

        if (point != vec4(0.0)) // check if not background
        {
            vec3 v = point.xyz - vert.xyz;
            float dist = length(v);

            if (dist < max_dist) // check distance in view space (remember model is normalized by modelmatrix)
            {
                float dist_factor = 1.0 / (1.0 + dist);
                occlusion += max(0.0, dot (normal, v)) * dist_factor * intensity;
            }
        }
    }

    occlusion = 1 - (occlusion / float(numberOfSamples));

    return occlusion;
}

void main (void)
{
    vec4 vert = texelFetch(coordsTexture, ivec2(gl_FragCoord.xy), 0);

    // if background pixel, discard
    if (vert.w == 0.0)
        discard;

    vec3 normal = texelFetch(normalTexture, ivec2(gl_FragCoord.xy), 0).xyz;

    // compute ambient occlusion
    float occlusion = ambientOcclusion(vert.xyz, normal);

    out_Color = vec4(1.0) * occlusion;

}
