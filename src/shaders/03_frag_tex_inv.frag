#version 330 core

in vec2 uvCoords;

out vec4 fragmentColor;

uniform sampler2D tex;
uniform sampler2D anotherTex;

void main() {
    vec2 newUV = vec2(1.0f - uvCoords.x, uvCoords.y);
    fragmentColor = mix(texture(tex, newUV), texture(anotherTex, newUV), 0.4f);
}