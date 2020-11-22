#version 330 core

in vec2 uvCoords;

out vec4 fragmentColor;

uniform sampler2D tex;
uniform sampler2D anotherTex;

void main() {
    fragmentColor = mix(texture(tex, uvCoords), texture(anotherTex, uvCoords), 0.4);
}