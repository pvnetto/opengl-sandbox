#version 330 core

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} inFragment;

layout (location = 0) out vec4 outPrimaryColor;
layout (location = 1) out vec4 outSecondaryColor;

uniform int bIsHorizontal;
uniform sampler2D primaryTexture;
uniform sampler2D secondaryTexture;

const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

const float gamma = 2.2;
vec3 textureLinear(in sampler2D inTexture, in vec2 inUV) {
    return pow(texture(inTexture, inUV).rgb, vec3(gamma));
}

void main() {
    // calculates the normalized size of each texel, relative to the texture's dimensions
    const int lod = 0;
    vec2 texelSize = vec2(1.0) / textureSize(primaryTexture, lod);

    // reads from primary, writes to secondary
    if(bIsHorizontal == 0)
    {
        vec3 result = textureLinear(primaryTexture, inFragment.uv) * weight[0];
        
        for (int i = 1; i < 5; ++i)
        {
            result += textureLinear(primaryTexture, inFragment.uv + vec2(texelSize.x * i, 0.0)) * weight[i];
            result += textureLinear(primaryTexture, inFragment.uv - vec2(texelSize.x * i, 0.0)) * weight[i];
        }

        outPrimaryColor = texture(primaryTexture, inFragment.uv);
        outSecondaryColor = vec4(result, 1.0);
        outSecondaryColor = pow(outSecondaryColor, vec4(1.0 / gamma));
    }
    // reads from secondary, writes to primary
    else
    {
        vec3 result = textureLinear(secondaryTexture, inFragment.uv) * weight[0];

        for (int i = 1; i < 5; ++i)
        {
            result += textureLinear(secondaryTexture, inFragment.uv + vec2(0.0, texelSize.y * i)) * weight[i];
            result += textureLinear(secondaryTexture, inFragment.uv - vec2(0.0, texelSize.y * i)) * weight[i];
        }

        outPrimaryColor = vec4(result, 1.0);
        outPrimaryColor = pow(outPrimaryColor, vec4(1.0 / gamma));
        outSecondaryColor = texture(secondaryTexture, inFragment.uv);
    }
}
