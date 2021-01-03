// Light sources should all emmit a solid color, they shouldn't be affected by lighting
#version 330 core

out vec4 fragmentColor;

uniform vec3 lightColor;

void main() {
    fragmentColor = vec4(lightColor, 1.0f);
}