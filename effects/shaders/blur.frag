#version 430
#define pi 3,14159265

//Gaussian Blur with Standard Deviation = 1

uniform vec2 texelSize;

uniform sampler2D blurTexture;
uniform ivec2 viewportSize;
uniform int blurRange;

out vec4 out_Color;

void main() {

    vec3 result = vec3(0.0);
    vec2 texCoord = vec2(gl_FragCoord.xy/vec2(viewportSize));

    //Weights for Gaussian Blur:
    float weight;
    float weightSum = 0.0;

    vec4 pixelColor = texelFetch(blurTexture, ivec2(gl_FragCoord.xy), 0);
    out_Color = vec4(pixelColor.rgb, 1.0);

//    for(int i = -(blurRange-1)/2 ; i <= (blurRange-1)/2 ; i++)
//    {
//        for(int j = -(blurRange-1)/2 ; j <= (blurRange-1)/2 ; j++)
//        {
//            vec2 offset = vec2(texelSize.x * float(i), texelSize.y * float(j));
//            weight = (1.0/2.0*pi) * exp(-0.5*(offset.x*offset.x + offset.y*offset.y));
//            weight = 1;
//            vec4 pixelColor = texture(blurTexture, texCoord + offset);
//            weight *= pixelColor.a;
//            result += pixelColor.rgb * weight;
//            weightSum += weight;
//        }
//    }

//    result /= weightSum;

//    out_Color = vec4(result,1.0);

}
