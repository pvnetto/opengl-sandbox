#version 330 core

in vec2 fragUV;
out vec4 fragColor;

uniform sampler2D renderTargetTexture;

void main() {
    // Applies grayscale filter to Render Target Texture
    fragColor = texture(renderTargetTexture, fragUV);
    float average = (fragColor.r + fragColor.g + fragColor.b) / 3.0;
    fragColor = vec4(average, average, average, 1.0);
}
