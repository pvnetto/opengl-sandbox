#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUVCoords;
layout (location = 2) in vec3 inNormal;

out vec3 worldPosition;
out vec2 uvCoords;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    
    worldPosition = vec3(model * vec4(inPosition, 1.0f));
    uvCoords = inUVCoords;
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    normal = normalMatrix * inNormal;
}