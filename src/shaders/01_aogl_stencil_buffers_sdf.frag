#version 330 core

out vec4 fragColor;

uniform vec2 inResolution;
uniform float inTime;

float circleSDF(vec2 position, float radius) {
    float dist = distance(position, vec2(0.5));
    float pct = 1.0 - smoothstep(radius, radius + 0.01, dist );
    return pct;
}

void main()
{
    // Normalizes pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy / inResolution.xy;
    
    // Fixes aspect-ratio
    uv.x *= (inResolution.x / inResolution.y);
    
    vec2 circlePosition = uv - vec2(0.4, 0.0);
    float circleSize = (cos(inTime * 1.5) + 1.0) * 0.5;
    float circle = circleSDF(circlePosition, circleSize);
    float bg = max(1.0 - circle, 0.0);
    
    // Discarded fragments don't write any values to the Framebuffer (includes Color, Depth and Stencil)
    if (circle > 0.0) {
        discard;
    }
    
    vec3 circleColor = circle * vec3(0.8, 0.3, 0.6);
    vec3 backgroundColor = bg * vec3(0.12, 0.135, 0.135);
    fragColor = vec4(circleColor + backgroundColor, 1.0);
}