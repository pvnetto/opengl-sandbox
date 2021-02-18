// Fractal Brownian Motion: a.k.a Fractal Noise, consists in layering multiple noise values
// by adding them iteratively and changing their frequency/amplitude values by an uniform constant.
// Each layer is called an octave, the constant for change in amplitude is called gain, and the one
// for change in frequency is called lacunarity.

// The fact each layer is called octave is not a coincidence, this works just like musical octaves. In
// music, each time a scale goes an octave up/down its frequency value is either doubled or halved, but
// in FBMs this ratio is not necessarily true, and this change could happen at a different rate.

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

float fbm(in vec2 p) {
    const int octaves = 4;
    const float gain = 0.5;        // amplitude change over octaves
    const float lacunarity = 2.;   // frequency change over octaves
    
    float freq = 1.;
    float amp = 0.5;

    float n = 0.;
    for(int i = 0; i < octaves; i++) {
        n += noise(p * freq) * amp;     // adds an octave to noise value
        
        freq *= lacunarity;             // uniform change in frequency
        amp *= gain;                    // uniform change in amplitude
    }
    
    return n;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    uv.x *= (iResolution.x / iResolution.y);

    vec2 p = uv * 4.;

    vec3 col = vec3(0.);
    col = vec3(fbm(p));
    col = mix(vec3(0.8, 0.8, 0.8), vec3(0.1, 0.2, 0.8), fbm(p));

    fragColor = vec4(col,1.0);
}