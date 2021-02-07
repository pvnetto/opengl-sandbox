#define PI 3.14159625

vec2 scale2d(vec2 p, vec2 s) {
    mat2 smat = mat2(s.x, 0.,
                     0., s.y);
    return smat * p;
}

vec2 rot2d(vec2 p, float ang) {
    mat2 rot = mat2(
        cos(ang), -sin(ang),
        sin(ang), cos(ang)
    );
    return rot * p;
}

vec2 translate2d(vec2 p, vec2 m) {
    mat3 trans = mat3(
        1.,  0.,  0.,
        0.,  1.,  0.,
        m.x, m.y, 1.
    );
    
    vec3 p3 = vec3(p.xy, 1.0);
    vec3 ptrans = trans * p3;
    
    p = vec2(ptrans.xy);
    
    return p;
}

vec3 box(vec2 p, vec2 size, vec3 color) {
    vec2 bounds = vec2(0.5) - (size / 2.0);
    
    vec2 blc = smoothstep(bounds, bounds + vec2(0.01), p);  // bottom-left
    float pct = blc.x * blc.y;
    
    vec2 trc = smoothstep(bounds, bounds + vec2(0.01), vec2(1.0) - p);  // top-right
    pct *= trc.x * trc.y;
    
    return pct * color;
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    //uv = uv * 2.0 - 1.0;
    //uv.x *= (iResolution.x / iResolution.y);
    
    vec2 p = uv;
    p -= vec2(0.5);     // Moves space from 0.5 to 0.0
    p = translate2d(p, vec2(0.1));
    p = rot2d(p, mod(iTime, 360.0));
    p = scale2d(p, vec2(0.8));
    p += vec2(0.5);     // Moves space back to 0.5
    
    // Time varying pixel color
    vec3 col = box(p, vec2(0.3), vec3(0.8, 0.2, 0.4));

    // Output to screen
    fragColor = vec4(col,1.0);
}