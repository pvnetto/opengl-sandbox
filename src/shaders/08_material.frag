#version 330 core

struct Material {
    vec3 color;

    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    vec3 specularColor;     // This might be used for specular mapping
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
};

out vec4 fragmentColor;

in vec3 worldPosition;
in vec3 normal;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {
    // calculates ambient
    vec3 ambient = material.ambientStrength * light.color;

    // calculates diffuse
    vec3 lightDir = normalize(light.position - worldPosition);
    vec3 diffuse = max(dot(lightDir, normal), 0.0f) * light.color * material.diffuseStrength;

    // calculates specular
    vec3 viewDir = normalize(viewPos - worldPosition);
    vec3 lightDirReflected = reflect(-lightDir, normal);
    float specularIntensity = max(dot(viewDir, lightDirReflected), 0.0f);
    specularIntensity = pow(specularIntensity, material.shininess);
    vec3 specular = specularIntensity * material.specularColor * material.specularStrength;

    // adds everything up
    fragmentColor = vec4((ambient + diffuse + specular) * material.color, 1.0f);
}