#define PI 3.14159265

float box(vec2 p, float size) {
    float pct = abs(p.x) + abs(p.y);
    return 1.0 - step(size, pct);
}

float circle(vec2 p, float radius) {
    return 1.0 - step(radius, length(p));
}

vec2 tile(vec2 p, in float zoom) {
    p *= zoom;
    return fract(p);
}

vec2 rotate2d(vec2 p, float ang) {
    p -= vec2(0.5);

    float s = sin(ang);
    float c = cos(ang);
    
    mat2 rot = mat2(
        c, -s,
        s, c
    );
    
    p = rot * p;
    
    p += vec2(0.5);
    
    return p;
}

// applies truchet tile pattern, where single design elements are drawn differently according to tile position
vec2 tilePattern(vec2 p) {
    // p is multiplied by 2 to create a 2x2 grid
    // 0  |  1
    // -------
    // 2  |  3
    
    p *= 3.0;
    
    float index = 0.0;  // ranges from 1 to 3
    index += step(1., mod(p.x, 2.0));
    index += step(1., mod(p.y, 2.0)) * 2.0;
    
    p = fract(p);  // moves space back to 1x1
    
    float rots[4] = float[4](0.f, PI * 0.5, PI * -0.5, PI);
    
    int idx = int(floor(index));
    
    return rotate2d(p, rots[idx]);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    uv = uv * 2.0 - vec2(1.0);
    uv.x *= (iResolution.x / iResolution.y);
    
    vec2 p = uv;
    p = tile(p, 3.0);
    p = tilePattern(p);

    // Time varying pixel color
    vec3 col = vec3(step(p.x, p.y));
    //col = vec3(box(p, 0.5));

    // Output to screen
    fragColor = vec4(col, 1.0);
}