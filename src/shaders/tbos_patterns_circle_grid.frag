float circle(vec2 p, float radius) {
    float c = distance(p, vec2(0.5));
    return 1.0 - step(radius, c);
}

vec2 tiling(vec2 p, float zoom) {
    p*= zoom;
    
    return fract(p);
}

vec2 tilingAnim(vec2 p, float zoom) {
    p *= zoom;
    
    float dir = mod(iTime, 2.0) - 1.0;
    float rowDir = step(1.0, mod(p.y, 2.0)) * 2.0 - 1.0;
    float colDir = step(1.0, mod(p.x, 2.0)) * 2.0 - 1.0;
    
    vec2 offset = vec2(max(dir, 0.0) * rowDir, min(dir, 0.0) * colDir);
    p += offset;
    
    return fract(p);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy;
    uv = uv * 2.0 - vec2(1.0);
    uv.x *= iResolution.x / iResolution.y;
    
    vec2 p = uv;
    p = tilingAnim(p, 5.0);
    vec3 col = circle(p, 0.3) * ((cos(iTime + uv.xyx + vec3(0,1,4)) + 1.0) / 2.0);

    fragColor = vec4(col, 1.0);
}