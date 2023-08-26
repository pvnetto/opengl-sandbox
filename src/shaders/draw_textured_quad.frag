#version 330 core

in vec2 fragUV;
out vec4 fragColor;

uniform sampler2D renderTargetTexture;

void main() {
    fragColor = texture(renderTargetTexture, fragUV);
}
