#version 330 core

layout (location = 0) in vec3 aPosition;

uniform float horizontalOffset;

void main() {
    gl_Position = vec4(aPosition.x + horizontalOffset, aPosition.yz, 1.0f);
}