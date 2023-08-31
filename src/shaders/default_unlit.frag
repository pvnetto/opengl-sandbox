#version 330 core

in vec2 uvCoords;

out vec4 fragColor;

uniform vec4 color;

void main() {
    if (color.a <= 0.01) {
        discard;
    } 

    fragColor = color;
}