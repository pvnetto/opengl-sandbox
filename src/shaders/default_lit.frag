#version 330 core

in vec3 worldPosition;
in vec2 fragUV;
in vec3 normal;

out vec4 fragmentColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float specularStrength;
uniform float specularShininess;        // should be a power of 2 
uniform vec3 viewPos;
uniform float tiling;
uniform float ambientIntensity;

uniform sampler2D albedo;

void main() {
    vec3 objectColor = texture(albedo, fragUV * tiling).rgb;

    // calculates ambient
    vec3 ambient = ambientIntensity * objectColor;

    // calculates attenuation as distance squared
    // alternatively we could go with:
    // attenuation = 1 / (constantCoefficient + linearCoefficient * distance + quadraticCoefficient * distance^2)
    float distanceFromLight = length(lightPos - worldPosition);
    float attenuation = 1.0f / (distanceFromLight * distanceFromLight);

    // calculates diffuse
    vec3 lightDir = normalize(lightPos - worldPosition);
    vec3 diffuse = max(dot(lightDir, normal), 0.0f) * lightColor * attenuation;

    // calculates specular
    vec3 viewDir = normalize(viewPos - worldPosition);
    vec3 lightDirReflected = reflect(-lightDir, normal);
    
    float specularIntensity = max(dot(lightDirReflected, viewDir), 0.0f);
    specularIntensity = pow(specularIntensity, specularShininess);
    vec3 specular = specularStrength * specularIntensity * lightColor * attenuation;

    // combines all components
    fragmentColor = vec4((ambient + diffuse + specular) * objectColor, 1.0f);
}