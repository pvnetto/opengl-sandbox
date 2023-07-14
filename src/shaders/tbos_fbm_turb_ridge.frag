// Turbulence: Works like a regular FBM, except noise is replace by abs(signed noise)
// Ridge: Variation of turbulence with inverted color and sharper creases

float rand(in vec2 p) {
    return fract(
        cos(
            dot(p, vec2(63510.65, 9452.32))
        ) * 34854.23
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

float snoise(in vec2 p) {
    return noise(p) * 2. - 1.;
}

float turbulence(in vec2 p) {
    const int octaves = 4;
    const float gain = 0.5;
    const float lacunarity = 2.;
    
    float freq = 2.;
    float amp = 1.;
    
    float n = 0.;
    for(int i = 0; i < octaves; i++) {
        n += abs(snoise(p * freq) * amp);
    
        freq *= lacunarity;
        amp *= gain;
    }
    
    return n;
}

float ridge(in vec2 p, in float offset) {
    // inverts turbulence noise by offset [0, 1]
    float n = offset - turbulence(p);
    // sharpens creases
    n = n * n;
    return n;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    uv.x *= (iResolution.x / iResolution.y);

    vec2 p = uv * 8.;

    vec3 col = vec3(0.);
    col = vec3(ridge(p, 1.));

    fragColor = vec4(col,1.0);
}