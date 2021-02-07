#define TWO_PI 6.28318530718
#define PI_OVER_TWO 1.57079633

//  Function from Iñigo Quiles
//  https://www.shadertoy.com/view/MsS3Wc
vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(
                   abs(
                       mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0) 
                   -3.0) - 1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix( vec3(1.0), rgb, c.y);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 st = gl_FragCoord.xy/iResolution.xy;
    st = st * 2.0 - vec2(1.0);
    st.x *= iResolution.x /iResolution.y;
    
    vec2 p = st;
    
    float radius = 0.8;
    float pct = 1.0 - smoothstep(radius, radius + 0.02, length(p));
    float pctw = 1.0 - smoothstep(radius + 0.04, radius + 0.06, length(p)) - pct;
    
    float hue = atan(p.y, p.x) / TWO_PI;   // maps angle from [-pi, pi] to [-0.5, 0.5]
    hue += 0.5;   // maps angle from [-0.5, 0.5] to [0.0, 1.0]
    
    float mag = 2.0 / radius;
    vec3 color = pct * hsb2rgb(vec3(hue, length(p) * mag, 1.0));
    color += pctw * vec3(1.0);

    fragColor = vec4(color,1.0);
}