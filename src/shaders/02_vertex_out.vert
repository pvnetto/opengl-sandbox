#version 330 core

layout (location = 0) in vec3 inPosition;

out vec3 vertexPosition;

void main() {
    gl_Position = vec4(inPosition, 1.0f);
    vertexPosition = inPosition;
}