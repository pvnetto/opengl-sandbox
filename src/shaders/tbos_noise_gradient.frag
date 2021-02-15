// gradient noise function by inigo quillez
// https://www.shadertoy.com/view/XdXGW8

// gradient noise has higher quality than value noise and uses 2d directions for
// noise algorithm's tile corners, instead of scalar random values 

// random 2d direction vector
vec2 rand(vec2 p) {
    const vec2 k = vec2( 0.3183099, 0.3678794 );
    p = p*k + k.yx;
    return -1.0 + 2.0*fract( 16.0 * k*fract( p.x * p.y*(p.x + p.y)) );
}

float noise(vec2 p) {    
    vec2 i = floor(p);
    vec2 f = fract(p);
    
    // samples 4 corners of tile around a point
    vec2 ca = rand(i);
    vec2 caf = f;
    
    vec2 cb = rand(i + vec2(1., 0.));
    vec2 cbf = f - vec2(1., 0.);
    
    vec2 cc = rand(i + vec2(0., 1.));
    vec2 ccf = f - vec2(0., 1.);
    
    vec2 cd = rand(i + vec2(1., 1.));
    vec2 cdf = f - vec2(1., 1.);
        
    vec2 u = smoothstep(0., 1., f);
    
    return mix(
        mix(dot(ca, caf), dot(cb, cbf), u.x),
        mix(dot(cc, ccf), dot(cd, cdf), u.x),
        u.y
    );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;

    float f = noise(uv * 32.f);
    f = 0.5 + 0.5 * f;  // remaps noise to [0.5, 1] before drawing
    vec3 col = vec3(f);

    fragColor = vec4(col,1.0);
}