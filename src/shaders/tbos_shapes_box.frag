// step multiplication works like AND operator
vec3 box(vec2 p, vec2 size, vec3 color) {
    vec2 bounds = vec2(0.5) - (size / 2.0);
    
    vec2 blc = smoothstep(bounds, bounds + vec2(0.01), p);  // bottom-left
    float pct = blc.x * blc.y;
    
    vec2 trc = smoothstep(bounds, bounds + vec2(0.01), vec2(1.0) - p);  // top-right
    pct *= trc.x * trc.y;
    
    return pct * color;
}

// thickness ranges from 0 to 1
vec3 box_outline(vec2 p, vec2 size, vec3 color, float thickness) {
    return box(p, size, color) - box(p, size * vec2(1.0 - thickness), color);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord / iResolution.xy;
    
    vec2 bottomLeftCorner = smoothstep(vec2(0.1), vec2(0.12), uv);      // Boxes can be draw using either step or floor
    //vec2 bottomLeftCorner = floor(uv + vec2(0.9)); 
    float pct = bottomLeftCorner.x * bottomLeftCorner.y;
    
    vec2 topRightCorner = smoothstep(vec2(0.1), vec2(0.12), vec2(1.0) - uv);
    //vec2 topRightCorner = floor(uv - vec2(0.9));
    pct *= topRightCorner.x * topRightCorner.y;
    
    vec3 color = pct * vec3(1.0, 0.0, 1.0);

    // Output to screen
    fragColor = vec4(color, 1.0);
}