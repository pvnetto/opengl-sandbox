// maps a 2d point in space to a random number between [0, 1]
float rand(vec2 p) {
    return fract(
         cos(
             dot(p, vec2(42343.31, 4874.23))
         ) * 59324.4  
    );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    
    fragColor = vec4(vec3(rand(uv)),1.0);
}