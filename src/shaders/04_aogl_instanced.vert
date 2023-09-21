#version 450 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec4 inColor;
layout (location = 4) in vec3 inWorldPosition;

out vec4 fragColor;
out vec2 fragUV;

uniform mat4 view;
uniform mat4 projection;

void main() {
    mat4 model = mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(inWorldPosition.xyz, 1.0));

    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    fragUV = vec2(inUV.xy);
    fragColor = inColor;
}