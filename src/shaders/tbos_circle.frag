#version 330 core

out vec4 fragColor;

uniform float iTime;
uniform vec2 iResolution;

void main() {
    vec2 pixelNorm = (gl_FragCoord.xy / iResolution.xy) * 2.0 - vec2(1.0);
    pixelNorm.x *= iResolution.x / iResolution.y;   // applies aspect ratio

    float pixelLength = length(pixelNorm);
    float circle = 1.0 - smoothstep(0.5, 0.52, pixelLength);

    fragColor = vec4(circle * 0.8, 0, 0, 1.0);
}