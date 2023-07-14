float rand(in vec2 p) {
    return fract(
        cos(
            dot(p, vec2(9232.62, 64567.32))
        ) * 9841.32
    );
}

float noise(in vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    float ca = rand(i);
    float cb = rand(i + vec2(1., 0.));
    float cc = rand(i + vec2(0., 1.));
    float cd = rand(i + vec2(1., 1.));
    
    vec2 u = smoothstep(0., 1., f);
    
    return mix(
        mix(ca, cb, u.x),
        mix(cc, cd, u.x),
        u.y
    );
}

float circle(in vec2 p, float r) {
    return 1. -  smoothstep(
        r,
        r + 0.02,
        distance(p, vec2(0.))
    );
}

float circleOutline(in vec2 p, float r, float t) {
    return circle(p, r + t) - circle(p, r);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    uv = uv * 2. - 1.;
    uv.x *= (iResolution.x / iResolution.y);
    
    vec2 p = uv;
    vec3 col = vec3(circleOutline(p, .2 + noise(cos(p + iTime)), 0.01));
    fragColor = vec4(col,1.0);
}