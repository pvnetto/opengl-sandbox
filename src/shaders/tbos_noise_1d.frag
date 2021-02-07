#define PI 3.14159265

// 1d random
float rand(float x) {
    return fract(
        sin(x) * 58748.32
    );
}

// 1d noise
float noise(float x) {
    float i = floor(x);
    float f = fract(x);
    return smoothstep(rand(i), rand(i + 1.), smoothstep(0., 1., f));
}

float circle(in vec2 p, in float radius) {
    return 1. - step(radius, length(p));
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    uv = uv * 2.0 - vec2(1.0);
    uv.x *= (iResolution.x / iResolution.y);
    
    float r = 0.4 + (0.3 * noise(iTime));
    vec3 col = vec3(circle(uv, r) - circle(uv, r - 0.01));

    fragColor = vec4(col,1.0);
}