#version 330 core

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D emissionMap;

    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
};

out vec4 fragmentColor;

in vec3 worldPosition;
in vec2 uvCoords;
in vec3 normal;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform float time;

void main() {
    vec3 color = vec3(texture(material.diffuseMap, uvCoords));

    // calculates ambient
    vec3 ambient = material.ambientStrength * light.color * color;

    // calculates diffuse
    vec3 lightDir = normalize(light.position - worldPosition);
    vec3 diffuse = max(dot(lightDir, normal), 0.0f) * light.color * color * material.diffuseStrength;

    // calculates specular
    vec3 viewDir = normalize(viewPos - worldPosition);
    vec3 lightDirReflected = reflect(-lightDir, normal);
    float specularIntensity = max(dot(viewDir, lightDirReflected), 0.0f);
    specularIntensity = pow(specularIntensity, material.shininess);
    vec3 specularSample = vec3(texture(material.specularMap, uvCoords));
    vec3 specular = specularIntensity * light.color * specularSample * material.specularStrength;

    // calculates emission
    int emissionStrength = specularSample.r == 0 && specularSample.g == 0 && specularSample.b == 0 ? 1 : 0;     // Checks if texture is inside black part of specular map
    vec3 emissionSample = vec3(texture(material.emissionMap, uvCoords + vec2(0, time)));
    vec3 emission = emissionSample * emissionStrength;
    
    // calculates fragment color
    fragmentColor = vec4((ambient + diffuse + specular + emission), 1.0f);
}