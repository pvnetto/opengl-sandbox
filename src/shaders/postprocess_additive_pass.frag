#version 330 core

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} inFragment;

out vec4 outFragColor;

uniform sampler2D factorTextureA;
uniform sampler2D factorTextureB;

void main() {
    vec4 colorA = texture(factorTextureA, inFragment.uv);
    vec4 colorB = texture(factorTextureB, inFragment.uv);

    outFragColor = colorA + colorB;

    // Prevents bleeding when mixing HDR colors by clamping the result
    float colorMag = max(length(colorA), length(colorB));
    outFragColor = normalize(outFragColor) * colorMag;
}
