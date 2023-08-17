#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;

out vec2 uvCoords;

void main() {
    gl_Position = vec4(inPosition, 1.0f);
    uvCoords = inUV;
}