#version 330 core
#define PI 3.1415

out vec4 fragColor;

uniform float iTime;
uniform vec2 iResolution;


void main() {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    
    float r = pow(1.0 - smoothstep(0.0, 2.0 / 6.0, uv.x), 0.1);
    r += pow(smoothstep(5.0 / 6.0, 1.0, uv.x), 0.1);
    float g = pow(smoothstep(1.0 / 6.0, 4.0 / 6.0, uv.x) - step(4.0 / 6.0, uv.x), 0.1);
    float b = pow(smoothstep(3.0 / 6.0, 1.0, uv.x), 0.1);

    vec3 color = vec3(r, g, b);
    
    // Output to screen
    fragColor = vec4(color.xyz, 1.0);
}