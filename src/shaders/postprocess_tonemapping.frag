#version 330 core

in vec2 fragUV;
out vec4 fragColor;

uniform sampler2D renderTargetTexture;
uniform float gammaCorrectionRatio;
uniform int tonemappingType;
uniform float exposure;

void main() {
    vec3 color = texture(renderTargetTexture, fragUV).rgb;

    // Applies Reinhard tone mapping
    if (tonemappingType == 1) {
        color = color / (color + vec3(1.0));
    }
    // Applies Exposure tone mapping
    if (tonemappingType == 2) {
        color = vec3(1.0) - exp(-color * exposure);
    }
    
    // Applies gamma correction
    const float gamma = 2.2;
    color = pow(color, vec3(1.0 / gammaCorrectionRatio));
  
    fragColor = vec4(color, 1.0);
}
