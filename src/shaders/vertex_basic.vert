#version 330 core                               // OpenGL core version 3.3

layout (location = 0) in vec3 aPosition;        // Receives data from buffer at index 0

void main() {
    gl_Position = vec4(aPosition.x, aPosition.y, aPosition.z, 0.0);     // gl_Position is an internal variable for OpenGL position
};