// Domain Warping consists of using FBMs to modulate other FBMs.
// Implementation based on: https://www.iquilezles.org/www/articles/warp/warp.htm 

float rand(in vec2 p) {
    return fract(
        cos(
            dot(p, vec2(63510.65, 9452.32))
        ) * 34854.23
    );
}

vec2 rand2(in vec2 p) {
    const vec2 k = vec2( 0.3183099, 0.3678794 );
    p = p*k + k.yx;
    return -1.0 + 2.0*fract( 16.0 * k*fract( p.x * p.y*(p.x + p.y)) );
}

vec2 rand2f(in vec2 p) {
    return fract(rand2(p));
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

float fbm(in vec2 p) {
    const int octaves = 4;
    const float gain = 0.5;
    const float lacunarity = 2.;
    
    float freq = 1.;
    float amp = 1.;
    
    float n = 0.;
    for(int i = 0; i < octaves; i++) {
        n += noise(p * freq) * amp;
        
        freq *= lacunarity;
        amp *= gain;
    }
    
    return n;
}

// modulates fbm with multiple other fbms
float pattern(out vec2 q, out vec2 r, in vec2 p) {
    // q, r are passed as out parameters so they can be used to modulate color
    q = vec2(
        fbm(p + iTime * 0.4),
        fbm(p + vec2(4.))
    );
    
    r = vec2(
        fbm(p + q + vec2(6.12, 3.15) + 0.13 * iTime),
        fbm(p + q + vec2(1.15, 1.57) + 0.11 * iTime)
    );
    
    return fbm(p + r);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;

    vec2 q; vec2 r;
    float n = pattern(q, r, uv * 4.);

    vec3 col = vec3(0.);
    //col = vec3(n);
    
    // also modulates color with fbms
    col = mix(vec3(0.8, 0.8, 0.8), vec3(0.8, 0.1, 0.3), n);
    col = mix(col, vec3(0.2, 0.53, 0.8), q.x);
    col = mix(col, vec3(0.1, 0.2, 0.4), r.x * r.y);

    fragColor = vec4(col,1.0);
}