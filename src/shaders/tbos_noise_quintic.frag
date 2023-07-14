// uses quintic smoothing instead of cubic (smoothstep), which results
// in smoother transition between adjacent cells
vec2 quintic(vec2 v) {
    return v * v * v * (v * (v * 6. - 15.) + 10.);
}

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
    
    vec2 u = quintic(f);
    
    return mix(
        mix(ca, cb, u.x),
        mix(cc, cd, u.x),
        u.y
    );
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    
    vec2 p = uv * 8.;
    vec3 col = vec3(noise(p));
    fragColor = vec4(col,1.0);
}