// Tiled implementation of Celullar noise. A lot more efficient because
// we only have to compare each pixel with the 8 neighboring cells, instead of all points.
#define PI 3.14159265

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

    vec2 p = uv * 3.;
    vec2 p_i = floor(p);
    vec2 p_f = fract(p);
    
    vec3 col = vec3(0.);
    float minDist;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            vec2 neighbor = vec2(i, j);
            vec2 point = neighbor + rand2f(p_i + neighbor);
            point += 0.5 + 0.5 * sin(iTime + 2. * PI * point);  // animates point using PI, resulting in less artifacts
            
            float dist = distance(p_f, point);
            
            if(i == -1 && j == -1) minDist = dist; 
            
            minDist = min(minDist, dist);
        }
    }
    
    //col += vec3(minDist);                   // draws distance map
    //col += vec3(step(0.3, minDist));        // draws circles
    col += vec3(fract(sin(minDist) * 10.));   // draws isolines
    
    col += 1. - step(0.01, minDist);          // draws points
    col.r += step(.99, p_f.x) + step(.99, p_f.y);  // draws grid

    fragColor = vec4(col,1.0);
}