float box(vec2 p, in vec2 size) {
    vec2 bounds = 0.5 - size;
    
    vec2 blc = step(bounds, p);
    vec2 trc = step(bounds, vec2(1.0) - p);
    
    return blc.x * blc.y * trc.x * trc.y;
}

vec2 rotate2d(vec2 p, float ang) {
    mat2 rot = mat2(
        cos(ang), -sin(ang),
        sin(ang), cos(ang)
    );
    return rot * p;
}

vec2 scale2d(vec2 p, vec2 sc) {
    mat2 scale = mat2(
        sc.x, 0.,
        0., sc.y
    );
    
    return scale * p;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    
    float t = 4.;
    vec2 p = uv * t;
    uv = fract(p);
    uv -= vec2(0.5);
    uv = rotate2d(uv, radians(cos(iTime) * 360.f));
    uv = scale2d(uv, vec2(0.3 + abs(cos(iTime) * 0.5)));
    uv += vec2(0.5);

    // Time varying pixel color
    float b = box(uv, vec2(0.1));
    vec3 col = b * vec3(p.y / t, p.x / t,abs(cos(iTime / 3.)));

    // Output to screen
    fragColor = vec4(col,1.0);
}