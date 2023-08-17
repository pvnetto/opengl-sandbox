#version 330 core

layout (location = 0) in vec3 inPosition;

uniform float horizontalOffset;

void main() {
    gl_Position = vec4(inPosition.x + horizontalOffset, inPosition.yz, 1.0f);
}