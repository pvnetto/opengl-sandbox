#version 450 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec4 inColor;
layout (location = 4) in mat4 inModel;

out vec4 fragColor;
out vec2 fragUV;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * inModel * vec4(inPosition, 1.0f);
    fragUV = vec2(inUV.xy);
    fragColor = inColor;
}