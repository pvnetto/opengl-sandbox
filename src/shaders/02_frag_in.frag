#version 330 core

in vec3 vertexPosition;
out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(vertexPosition, 1.0f);
}