// Very simple lighting shader, multiplies albedo by the lighting color,
// doesn't consider distance, angle or anything, only the light's color.
#version 330 core

out vec4 fragmentColor;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
    fragmentColor = vec4(lightColor * objectColor, 1.0f);
}