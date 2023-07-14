void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    uv = (uv * 2.0) - vec2(1.0);
    uv.x *= iResolution.x / iResolution.y;
    
    float pct = length(abs(uv));
    pct += mix( 0.5, -2.0, mod(iTime * 1.3, 1.0));
    
    // Output to screen
    vec3 solidColor = vec3(0.9, 0.7, 0.1);
    fragColor = vec4(vec3(smoothstep(0.3, 0.4, pct) * smoothstep(0.6, 0.4, pct) ) * vec3(0.9, 0.5, 0.3) ,1.0);
}