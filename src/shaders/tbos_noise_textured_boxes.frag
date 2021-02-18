float rand(in vec2 p) {
    return fract(
        cos(
            dot(p, vec2(5432.24, 9873.68))
        ) * 9587.32
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

float box(in vec2 p, in vec2 size) {
    vec2 bounds = 0.5 - size;
    vec2 blc = step(bounds, p);
    vec2 trc = step(bounds, 1. - p);
    
    return blc.x * blc.y * trc.x * trc.y;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    vec2 p = uv * 32.;
    
    vec2 S = vec2(0.6, 0.3);
    S -= noise(p) * 0.08;
    float pct = box(fract(uv * vec2(1., 6.)), S);
    
    vec3 col = mix(vec3(0.25, 0.25, 0.25), vec3(0.2, 0.7, 0.7), pct);

    fragColor = vec4(col,1.0);
}