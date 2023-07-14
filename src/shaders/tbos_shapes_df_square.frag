void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    uv = (uv * 2.0) - vec2(1.0);
    uv.x *= iResolution.x / iResolution.y;
    
    float pct = length(max(abs(uv) - vec2(0.3), 0.0));
    
    // Output to screen
    fragColor = vec4(vec3(step(0.3, pct)) ,1.0);
}