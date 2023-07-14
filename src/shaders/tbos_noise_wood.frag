// creates simple wood-ish pattern, based on https://thebookofshaders.com/edit.php#11/wood.frag
// but a lot simplified, so the lines function is more intuitive

#define PI 3.14159265

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

vec2 rotate2d(in vec2 p, in float ang) {
    float c = cos(ang);
    float s = sin(ang);
    
    mat2 rot = mat2(
        c, -s,
        s, c
    );
    
    return rot * p;
}

// creates vertical line pattern by modulating x axis with sine
float lines(in vec2 p, float w) {
    float scale = 10.;
    p *= scale;
    
    return smoothstep(
        0., 0.5, abs(sin(p.x))
    );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;

    vec2 p = uv * 4.;
    p = rotate2d(p, noise(p));   // rotates p by random value

    vec3 col = vec3(lines(p, .5));
    fragColor = vec4(col,1.0);
}