#version 330 core

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} inFragment;

out vec4 fragColor;

uniform sampler2D renderTargetTexture;
uniform float gammaCorrectionRatio;
uniform int tonemappingType;
uniform float exposure;

const float gamma = 2.2;

void main() {
    // Converts texture color from sRGB (gamma-encoded) to linear space
    vec3 color = texture(renderTargetTexture, inFragment.uv).rgb;
    color = pow(color, vec3(gamma));

    // Applies Reinhard tone mapping
    if (tonemappingType == 1) {
        color = color / (color + vec3(1.0));
    }
    // Applies Exposure tone mapping
    if (tonemappingType == 2) {
        color = vec3(1.0) - exp(-color * exposure);
    }
    
    // Outputs gamma-corrected color
    fragColor = pow(vec4(color.rgb, 1.0), vec4(1.0 / gamma));
}
