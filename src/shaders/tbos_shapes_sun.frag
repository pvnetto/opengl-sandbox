void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    uv = (uv * 2.0) - vec2(1.0);
    uv.x *= (iResolution.x / iResolution.y);
    
    // sun uses polar coordinates to modulate a distance field
    vec2 point = uv - vec2(0.3);
    float rad = length(point);
    float ang = atan(point.y, point.x);
    
    float pct = sin(mod(ang * 8.0 + iTime, 0.5));
    pct += cos(ang * 8.0);
    
    float pctB = 0.3;
    pctB += pow(cos(abs(fract(ang + iTime * 0.3)) * 4.0), 2.0) * 0.01;
    
    // background using distance fields
    float bgA = max(length(uv + vec2(0.5)), 0.43);
    float bgB = max(length(uv - vec2(0.5)), 0.5);
    float pctdf = 1.0 - max(bgA * bgB * 0.3 * fract(mod(rad, 0.8) * 0.9), 0.3);
    
    // twirly eye
    vec2 twirlyPos = point + vec2(0.1, -0.1);
    float twirlyRad = length(twirlyPos);
    float twirly = fract(twirlyRad * 40.0);
    twirly = step(0.3, twirly);
    float st = 1.0 - step(0.1, twirlyRad);
    twirly += mod(iTime, 2.0) * 0.1;
    vec3 twirlyCol = vec3(twirly) * st;
    
    float circle = 1.0 - smoothstep(pct, pct + 0.2, rad);
    circle += 1.0 - smoothstep(pctB, pctB, rad);
    
    vec3 color = circle * vec3(1.0, 1.0, rad * 0.3);
    pctdf *= (1.0 - circle);
    color += mix(vec3(1.0, 0.6, 0.2), vec3(1.0, 0.80, 0.8), pctdf);
    color += twirlyCol;
    
    // Output to screen
    fragColor = vec4(color,1.0);
}