// Naïve implementation of cellular noise, compares pixel with all points. 
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;

    const int N = 5;
    
    vec2 points[N];
    points[0] = vec2(0.1, 0.2);
    points[1] = vec2(0.8, 0.2);
    points[2] = vec2(0.4, 0.4);
    points[3] = vec2(0.4, 0.8);
    points[4] = iMouse.xy / iResolution.xy;

    float minDist = 9999.f;
    
    vec3 col = vec3(0.);
    
    for(int i = 0; i < N; i++) {
        float dist = distance(uv, points[i]);
        minDist = min(dist, minDist);
    }

    col += vec3(minDist);
    col += minDist <= 0.005 ? vec3(1.0) : vec3(0.);    // draws points

    fragColor = vec4(col,1.0);
}