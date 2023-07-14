#version 330 core
#define PI 3.1415

out vec4 fragColor;

uniform float iTime;
uniform vec2 iResolution;

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    uv = uv * 2.0 - vec2(1.0);
    uv.x *= iResolution.x / iResolution.y;
    
    vec3 energized = vec3(0.9, 0.7, 0.1);
    vec3 anxious = vec3(1.0, 0.1, 0.3);
    
    float calmness = (cos(iTime / 2.0f) + 1.0) / 2.0;
    vec3 color = mix(energized, anxious, vec3(mix(1.0, 0.7, calmness), calmness, 1.0));
    
    float radius = 0.05f;
    int ballcount = 8;
    for(int i = 0; i < ballcount; i++){
        float progress = float(i) / float(ballcount); 
        float offset = progress * 2.0 * PI + iTime;
        
        vec2 circleCoords = vec2(cos(offset), sin(offset)) * 0.4;
        
        vec2 center = uv + circleCoords;
        float pLength = length(center) - abs(uv.x * exp(1.0 - abs(uv.x))) * calmness * 0.8;
        
        vec3 sphere = vec3(smoothstep(radius, radius + 0.04, pLength));
        sphere = vec3(calmness * 0.23) + sphere;

        color *= sphere;
    }
    
    // Output to screen
    fragColor = vec4(color, 1.0);
}