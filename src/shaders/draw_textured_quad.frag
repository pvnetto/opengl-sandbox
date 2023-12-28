#version 330 core

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} inFragment;

out vec4 fragColor;

uniform sampler2D renderTargetTexture;

void main() {
    fragColor = texture(renderTargetTexture, inFragment.uv);
}
