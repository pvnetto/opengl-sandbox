// View-space version of Phong shading model
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec3 inNormal;

out vec3 fragPos;
out vec3 normal;
out vec3 lightPos;

uniform vec3 lightPosWorld;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    
    fragPos = vec3(view * model * vec4(inPosition, 1.0f));
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    normal = normalize(normalMatrix * inNormal);
    lightPos = vec3(view * vec4(lightPosWorld, 1.0f));
}