// Draws depth buffer to the screen
#version 330 core

in vec2 fragUV;
out vec4 fragColor;

uniform float nearPlane;
uniform float farPlane;

// When the View matrix is applied, resulting depth values range from [nearPlane, farPlane]. They need to be normalized though,
// as the Depth Buffer stores values from [0, 1].
// 
// So we apply the Projection matrix, which takes the depth values in [nearPlane, farPlane] range, transforms them to a screen-space
// z-coordinate that ranges from [0, 1], and stores then in gl_FragCoord.z.
//
// This is all fine and dandy, except that the calculation that converts from view to screen-space is non-linear, that is, smaller
// depth values have much more precision than bigger depth values, as we care much more about depth of nearby objects. That function
// follows a similar curve to 1 / z, with z values between [1.0, 2.0] covering half the [0, 1] range
// 
// f(z) = 1 / 1 = 1 
// f(z) = 1 / 2 = 0.5
// f(z) = 1 / 10 = 0.1
// ...
// f(z) = 1 / 100 = 0.01
// f(z) = 1 / 101 = 0.009900990099
// 
// Note how fast f(z) changes for smaller values, and how little change there is from 100 to 101 (approx. 0.001).
// 
// While this is a good property for rendering in general, it might not be the best option for visualizing depth values,
// so here we'll get those non-linear screen-space z-coordinates in the [0, 1] range and convert them to a linear space.

// This equation is derived from the Projection matrix portion that generates non-linear depth values
// Check: https://learnopengl.com/Advanced-OpenGL/Depth-testing
float LinearizeDepth(float depth) {
    float ndcDepth = depth * 2.0 - 1.0;                                 // [0, 1] => [-1, 1]

    float projectionPlaneLength = farPlane - nearPlane;                 // Distance between near and far planes
    float depthProjectionPosition = ndcDepth * projectionPlaneLength;   // [-1, 1] => [-planeLength, planeLength]

    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - depthProjectionPosition);     // No idea, math heavy explanation
}

void main() {
    // Depth values are stored in gl_FragCoord.z, and also gl_FragDepth if we don't write anything to it manually
    vec3 linearDepthColor = vec3(LinearizeDepth(gl_FragCoord.z));
    fragColor = vec4(linearDepthColor, 1.0);
}