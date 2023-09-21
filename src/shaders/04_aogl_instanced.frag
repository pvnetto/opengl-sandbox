#version 450 core

in vec4 fragColor;
in vec2 fragUV;

out vec4 outColor;

void main() {
    if (fragColor.a <= 0.01) {
        discard;
    } 

    outColor = fragColor;
}