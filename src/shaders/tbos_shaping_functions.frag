#version 330 core

out vec4 fragColor;

uniform float iTime;
uniform vec2 iResolution;

#define PI 3.1415

// plotting functions return 1 when x = y

// float plot(vec2 point, float px) {   // returns diff between upper curve and slightly smaller upper curve
//     return smoothstep(px - 0.02, px, point.y) - smoothstep(px, px + 0.02, point.y);
// }

float plot(vec2 point, float px) {
    return 1.0 - smoothstep(0.0, 0.02, abs(point.y - px));
}

void main() {
    vec2 pixelCoord = (gl_FragCoord.xy / iResolution.xy);
    vec2 pixelNorm = pixelCoord * 2.0 - vec2(1.0);

    float pixelPlot = 0;
    float px = 1.0 - pow(abs(pixelNorm.x), 2);
    // pixelPlot += plot(pixelCoord, pixelCoord.x);                             // linear
    // pixelPlot += plot(pixelCoord, pow(pixelCoord.x, 5));                     // quadratic
    // pixelPlot += plot(pixelNorm, cos(pixelNorm.x * PI * 3 + iTime));         // cos
    // pixelPlot += plot(pixelNorm, sin(pixelNorm.x * PI * 3 + iTime));         // sin
    // pixelPlot += plot(pixelCoord, abs(sin(pixelCoord.x * PI * 3 + iTime)));  // sin abs
    pixelPlot += plot(pixelCoord, 1.0 - pow(abs(pixelNorm.x), 2));              // parabola

    // pixelPlot += plot(pixelCoord, fract(pixelCoord.x * 5));
    // pixelPlot += plot(pixelCoord, sign(pixelCoord.x));
    // pixelPlot += plot(pixelCoord, max(pixelCoord.x, 0.0));
    // pixelPlot += plot(pixelCoord, min(pixelCoord.x, 0.0));
    
    vec3 gradient = (1.0 - pixelPlot) * vec3(px);
    vec3 color = gradient + pixelPlot * vec3(0.0, 0.8, 0.2); 

    fragColor = vec4(color, 1.0);
}