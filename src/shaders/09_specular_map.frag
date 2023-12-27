#version 330 core

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;

    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
};

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} inFragment;

out vec4 fragmentColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {
    vec3 color = vec3(texture(material.diffuseMap, inFragment.uv));

    // calculates ambient
    vec3 ambient = material.ambientStrength * light.color * color;

    // calculates diffuse
    vec3 lightDir = normalize(light.position - inFragment.worldPosition);
    vec3 diffuse = max(dot(lightDir, inFragment.normal), 0.0f) * light.color * color * material.diffuseStrength;

    // calculates specular
    vec3 viewDir = normalize(viewPos - inFragment.worldPosition);
    vec3 lightDirReflected = reflect(-lightDir, inFragment.normal);
    float specularIntensity = max(dot(viewDir, lightDirReflected), 0.0f);
    specularIntensity = pow(specularIntensity, material.shininess);
    vec3 specularSample = vec3(texture(material.specularMap, inFragment.uv));
    vec3 specular = specularIntensity * light.color * specularSample * material.specularStrength;
    
    // calculates fragment color
    fragmentColor = vec4((ambient + diffuse + specular), 1.0f);
}