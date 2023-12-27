#version 330 core

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;

    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} inFragment;

out vec4 fragmentColor;

uniform Material material;
uniform DirectionalLight light;
uniform vec3 viewPos;

void main() {
    vec3 diffuseSample = vec3(texture(material.diffuseMap, inFragment.uv));

    // calculates ambient
    vec3 ambient = material.ambientStrength * diffuseSample * light.color;

    // calculates diffuse
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = max(dot(lightDir, inFragment.normal), 0.0f) * diffuseSample * light.color;

    // calculates specular
    vec3 lightDirReflected = reflect(-lightDir, inFragment.normal);
    vec3 viewDir = normalize(viewPos - inFragment.worldPosition);
    float specularIntensity = max(dot(lightDirReflected, viewDir), 0.0f);
    specularIntensity = pow(specularIntensity, material.shininess);
    vec3 specularSample = vec3(texture(material.specularMap, inFragment.uv));
    vec3 specular = specularIntensity * material.specularStrength * specularSample * light.color;

    // calculates fragment color
    fragmentColor = vec4(ambient + diffuse + specular, 1.0f); 
}