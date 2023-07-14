void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    
    float pct = distance(uv, vec2(0.5));
    float st = 1.0 - step(0.35, pct);
    pct += (mod(iTime, 10.0)) * 0.2;

    // Output to screen
    fragColor = vec4(st * vec3(fract(pct * 12.0)) ,1.0);
}