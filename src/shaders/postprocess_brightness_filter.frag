#version 330 core

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} inFragment;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

uniform float brightnessThreshold;
uniform sampler2D renderTargetTexture;

const float gamma = 2.2;

void main() {
    // Converts texture color from sRGB (gamma-encoded) to linear space
    vec3 color = texture(renderTargetTexture, inFragment.uv).rgb;
    color = pow(color, vec3(gamma));

    // Checks how bright the fragment color is
    const vec3 brightnessKernel = vec3(0.2126, 0.7152, 0.0722);
    float fragmentBrightness = dot(color, brightnessKernel);
    float brightnessOutputFactor = fragmentBrightness >= brightnessThreshold ? 1.0f : 0.0f;
    
    // Calculates gamma-corrected color
    vec4 gammaCorrectedColor = pow(vec4(color.rgb, 1.0), vec4(1.0 / gamma));

    // Outputs fragment and bright color
    fragColor = gammaCorrectedColor;
    brightColor = gammaCorrectedColor * brightnessOutputFactor;
}
