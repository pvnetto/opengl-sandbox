vec3 background = vec3(0.3, 0.3, 0.3);

float circle(vec2 p, float radius) {
    float dist = distance(p, vec2(0.5));
    float pct = 1.0 - smoothstep(radius, radius + 0.01, dist );
    return pct;
}

vec3 circle(vec2 p, float radius, vec3 color) {
    float dist = distance(p, vec2(0.5));
    float pct = 1.0 - smoothstep(radius, radius + 0.01, dist );
    return pct * color;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord / iResolution.xy;
    uv.x *= (iResolution.x / iResolution.y);
    
    vec2 p = uv - vec2(0.4, 0.0);
    float circleA = circle(p, 0.2);
    float circleB = circle(p + vec2(0.6, 0.0), 0.2);
    float bg = max(1.0 - circleA - circleB, 0.0);
    
    vec3 color = circleA * vec3(0.8, 0.3, 0.6) + circleB * vec3(0.2, 0.7, 0.3) + bg * background;
    
    // Output to screen
    fragColor = vec4(color, 1.0);
}