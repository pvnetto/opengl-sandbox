#version 330 core

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} inFragment;

out vec4 fragmentColor;

uniform vec3 viewPos;

uniform sampler2D albedo;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientIntensity;
uniform float specularStrength;
uniform float specularShininess;        // should be a power of 2 

uniform float tiling;

void main() {
    // converts texture color from sRGB (gamma-encoded) to linear space
    vec3 objectColor = texture(albedo, inFragment.uv * tiling).rgb;
    objectColor = pow(objectColor, vec3(2.2));

    // calculates ambient
    vec3 ambient = ambientIntensity * objectColor;

    // calculates attenuation as distance squared
    // alternatively we could go with:
    // attenuation = 1 / (constantCoefficient + linearCoefficient * distance + quadraticCoefficient * distance^2)
    float distanceFromLight = length(lightPos - inFragment.worldPosition);
    float attenuation = 1.0f / (distanceFromLight * distanceFromLight);

    // calculates diffuse
    vec3 lightDir = normalize(lightPos - inFragment.worldPosition);
    vec3 diffuse = max(dot(lightDir, inFragment.normal), 0.0f) * lightColor * attenuation;

    // calculates specular
    vec3 viewDir = normalize(viewPos - inFragment.worldPosition);
    vec3 lightDirReflected = reflect(-lightDir, inFragment.normal);
    
    float specularIntensity = max(dot(lightDirReflected, viewDir), 0.0f);
    specularIntensity = pow(specularIntensity, specularShininess);
    vec3 specular = specularStrength * specularIntensity * lightColor * attenuation;

    // combines all components
    fragmentColor = vec4((ambient + diffuse + specular) * objectColor, 1.0f);
    
    // applies gamma correction
    fragmentColor = pow(fragmentColor, vec4(1.0 / 2.2));
}