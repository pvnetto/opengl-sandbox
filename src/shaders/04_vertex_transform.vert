#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;

out vec2 uvCoords;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(inPosition, 1.0f);
    uvCoords = vec2(inUV.x, -inUV.y);
}