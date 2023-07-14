// Multiple combinations between distance fields

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;

    float pct = distance(uv, vec2(0.4)) + distance(uv, vec2(0.6));
    // float pct = distance(uv, vec2(0.4)) * distance(uv, vec2(0.6));
    // float pct = pow(distance(uv, vec2(0.4)), distance(uv, vec2(0.6)));
    // float pct = min(distance(uv, vec2(0.4)), distance(uv, vec2(0.6)));
    // float pct = max(distance(uv, vec2(0.4)), distance(uv, vec2(0.6)));
    vec3 col = pct * vec3(0.8, 0.3, 0.4);

    // Output to screen
    fragColor = vec4(col,1.0);
}