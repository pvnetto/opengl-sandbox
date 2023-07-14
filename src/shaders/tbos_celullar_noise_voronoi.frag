// Draws a Voronoi Diagram, which is just a pattern where pixels are
// colored depending on their closest point. In this case we're assigning
// the rg channels of final color to equal the closest point, but we could also
// assign a particular color to each point.

vec2 rand2(in vec2 p) {
    const vec2 k = vec2( 0.3183099, 0.3678794 );
    p = p*k + k.yx;
    return -1.0 + 2.0*fract( 16.0 * k*fract( p.x * p.y*(p.x + p.y)) );
}

vec2 rand2f(in vec2 p) {
    return fract(rand2(p));
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;

    const float T = 2.;
    vec2 p = uv * T;
    vec2 p_i = floor(p);
    vec2 p_f = fract(p);
    
    vec3 col = vec3(0.);
    float minDist = 9999.;
    vec2 minPt;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            vec2 neighbor = vec2(float(i), float(j));
            vec2 pneighbor = p_i + neighbor;

            vec2 point = rand2f(p_i + neighbor);
            
            float dist = distance(p_f, point + neighbor);
            if(dist < minDist) {
                minDist = dist;
                minPt = point;
            }
        }
    }
    
    col += vec3(fract(sin(minDist) * 10.));
    col.rg = minPt;
    
    col += 1. - step(0.01, minDist);          // draws points

    fragColor = vec4(col,1.0);
}