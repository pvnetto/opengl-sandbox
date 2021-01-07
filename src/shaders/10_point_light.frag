#version 330 core

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;

    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
};

struct PointLight {
    vec3 position;
    vec3 color;

    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

in vec3 worldPosition;
in vec3 normal;
in vec2 uvCoords;

out vec4 fragmentColor;

uniform Material material;
uniform PointLight light;
uniform vec3 viewPos;

void main() {
    vec3 diffuseSample = vec3(texture(material.diffuseMap, uvCoords));

    float distanceFromLight = length(light.position - worldPosition);

    // calculates ambient
    vec3 ambient = material.ambientStrength * diffuseSample * light.color;

    // calculates diffuse
    vec3 lightDir = normalize(light.position - worldPosition);
    vec3 diffuse = max(dot(lightDir, normal), 0.0f) * diffuseSample * light.color;

    // calculates specular
    vec3 lightDirReflected = reflect(-lightDir, normal);
    vec3 viewDir = normalize(viewPos - worldPosition);
    float specularIntensity = max(dot(lightDirReflected, viewDir), 0.0f);
    specularIntensity = pow(specularIntensity, material.shininess);
    vec3 specularSample = vec3(texture(material.specularMap, uvCoords));
    vec3 specular = specularIntensity * material.specularStrength * specularSample * light.color;

    // applies attenuation
    // attenuation = 1 / kc + kl*d + kq*d^2
    float attenuation = 1.0f / (
        light.constantAttenuation
        + light.linearAttenuation * distanceFromLight
        + light.quadraticAttenuation * pow(distanceFromLight, 2));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // calculates fragment color
    fragmentColor = vec4(ambient + diffuse + specular, 1.0f); 
}