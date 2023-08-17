#version 330 core                               // OpenGL core version 3.3

layout (location = 0) in vec3 inPosition;        // Receives data from buffer at index 0

void main() {
    gl_Position = vec4(inPosition, 1.0f);     // gl_Position is an internal variable for OpenGL position
}