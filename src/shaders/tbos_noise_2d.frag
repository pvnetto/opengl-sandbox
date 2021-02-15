float rand(in vec2 p) {
    return fract(
        cos(
            dot(p, vec2(42654.42, 38433.95))
        ) * 98492.34
    );
}

// based on https://www.shadertoy.com/view/4dS3Wd
// interpolates between 4 corners of a tile built from a given point.
// this is known as value noise, as it interpolates between random values
float noise(in vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    
    // uses integer portion of coordinate to sample tile colors
    // note: colores are sampled from integer part of the coordinate,
    // so the number of sampled colors depends on the range of p
    float ca = rand(i);
    float cb = rand(i + vec2(1., 0.));
    float cc = rand(i + vec2(0., 1.));
    float cd = rand(i + vec2(1., 1.));
    
    vec2 u = smoothstep(0., 1., f);
    
    // interpolates 4 corners
    return mix(
        mix(ca, cb, u.x),  // interpolates corner a, b
        mix(cc, cd, u.x),  // interpolates corner c, d
        u.y
    );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    
    vec2 p = uv * 20.;
    //p = uv;
    //p = uv * cos(iTime) * 15.;

    vec3 col = vec3(noise(p));

    fragColor = vec4(col,1.0);
}