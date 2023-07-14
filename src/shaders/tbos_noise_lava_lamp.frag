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

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    vec2 p = uv * 16.;
    
    // moving point
    vec2 pos = p + vec2(cos(iTime), sin(iTime)) * 0.5;
    vec2 vel = vec2(iTime * 0.1, iTime * 0.2);
    
    // adds noise from static and moving points
    float n = 0.5 * noise(p);
    n += noise(pos + vel) * 0.5;
    
    float pct = smoothstep(0.3, 0.35, n);
    vec3 col = mix(vec3(0.8, 0.4, 0.), vec3(0.1, 0.1, 0.1), pct);

    fragColor = vec4(col,1.0);
}