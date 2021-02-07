float rand(vec2 p) {
    return fract(
         cos(
             dot(p, vec2(42343.31, 4874.23))
         ) * 59324.4  
    );
}

vec2 truchet(in vec2 p, in float index) {
    index = fract((index - 0.5) * 2.0);
    
    if(index > 0.75) {
        p = vec2(1.0) - p;
    }
    else if(index > 0.5) {
        p = vec2(1.0 - p.x, p.y);
    }
    else if(index > 0.25) {
        p = 1.0 - vec2(1.0 - p.x, p.y);
    }
    
    return p;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    uv *= 20.0;
    
    vec2 posI = floor(uv);
    vec2 posF = fract(uv);
    
    vec2 tile = truchet(posF, rand(posI));
    float lTile = length(tile);
    float lTileInv = length(1. - tile);
    
    float pct = 0.0;
    //pct = rand(posI);                                     // no truchet, just tiling
    //pct = step(tile.x, tile.y);                           // truchet triangles
    //pct = smoothstep(tile.x - 0.3, tile.x, tile.y)
    //    - smoothstep(tile.x, tile.x + 0.3, tile.y);       // truchet maze
    pct = (step(lTile, 0.6) - step(lTile, 0.4))
        + (step(lTileInv, 0.6) - step(lTileInv, 0.4));      // truchet circle
    
    fragColor = vec4(vec3(pct),1.0);
}