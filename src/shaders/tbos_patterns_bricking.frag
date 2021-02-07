float box(vec2 p, in vec2 size) {
    vec2 _size = 0.5 - (size / 2.0);
    vec2 blc = step(_size, p);
    vec2 trc = step(_size, 1.0 - p);
    
    return blc.x * blc.y * trc.x * trc.y;
}

// works like regular tiling, except x is offset at odd y values
vec2 brickTiling(vec2 p, float zoom) {
    p *= zoom;
    
    p.x += step(1., mod(p.y, 2.)) * 0.5;  // mod checks if row (y) is odd
    
    return fract(p);
}

vec2 brickTilingAnim(vec2 p, float zoom) {
    p *= zoom;
    float offsetOdd = step(1.0, mod(p.y, 2.0));
    p.x += offsetOdd * 0.5;
    
    float dir = offsetOdd * 2.0 - 1.0;
    p.x += iTime * dir;
    
    return fract(p);
}

vec2 brickTilingAnimVert(vec2 p, float zoom) {
    p *= zoom;
    
    float oddCol = step(1.0, mod(p.x, 2.0)); 
    p.y += oddCol * 0.5;
    
    float dir = oddCol * 2.0 - 1.0;
    p.y += dir * iTime;
    
    return fract(p);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy;
    //vec2 p = brickTiling(uv, 5.0);
    //vec2 p = brickTilingAnim(uv, 5.0);
    vec2 p = brickTilingAnimVert(uv, 5.0);
    vec3 col = box(p, vec2(0.9)) *cos(iTime+p.xyx+vec3(0,2,4));

    fragColor = vec4(col, 1.0);
}