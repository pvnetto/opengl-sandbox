#version 330 core

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} inFragment;

out vec4 outFragmentColor;

uniform vec4 baseColor = vec4(0.0, 0.0, 0.0, 1.0);
uniform sampler2D glyph;
uniform int debug;

void main() {
    vec4 glyphColor = vec4(1.0, 1.0, 1.0, texture(glyph, inFragment.uv).r);
    vec4 finalColor = glyphColor * baseColor;

	outFragmentColor = debug > 0 ? baseColor : finalColor;
}