#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

// NOTE: This is an Interface Block. You can read about the matching patterns between shaders here:
// https://www.khronos.org/opengl/wiki/Shader_Compilation#Interface_matching
out VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} outVertexData;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);

    mat3 normalMatrix = mat3(transpose(inverse(model)));    // Rotates/scales the normal if necessary
    vec4 worldPosition4d = model * vec4(inPosition, 1.0f);

    outVertexData.worldPosition = vec3(worldPosition4d.xyz);
    outVertexData.uv = vec2(inUV.x, -inUV.y);
    outVertexData.normal = normalize(normalMatrix * inNormal);
}
