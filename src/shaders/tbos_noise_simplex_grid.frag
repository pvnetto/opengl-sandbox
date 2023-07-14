// Simplex noise is the evolution of the previous noise implementation which
// interpolated the 4 corners of a cell to generate noise. In this approach,
// the grid is instead formed by simplexes (triangles), because they're the simplest
// geometrical form, so we only need to interpolate 3 sides for each cell, instead of 4.

// This example only shows how to form a simplex grid. It doesn't go as far as using
// it to generate noise, which would be a little more complicated. Simplex noise implementations
// can be found on shadertoy.

vec2 skew(in vec2 p) {
    vec2 res = vec2(0.);
    res.x = 1.1547 * p.x;
    res.y = p.y + 0.5 * p.x;
    return res;
}

// subdivides grid cells into equilateral triangles
vec3 simplexGrid(in vec2 p) {
    vec3 res = vec3(0.);
    
    vec2 sk = fract(skew(p));
    if(sk.x > sk.y) {
        res.xy = 1.0 - vec2(sk.x, sk.y - sk.x);
        res.z = sk.y;
    }
    else {
        res.yz = 1.0 - vec2(sk.x - sk.y, sk.y);
        res.x = sk.x;
    }
    
    return fract(res);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    vec2 p = uv * 4.;
    
    vec3 col = vec3(0.);
    //col.rg = fract(p);
    //col.rg = fract(skew(p));
    col = simplexGrid(p);

    fragColor = vec4(col,1.0);
}