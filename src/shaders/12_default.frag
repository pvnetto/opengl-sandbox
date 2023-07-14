#version 330 core

struct Material {
    sampler2D texture0;
    sampler2D texture1;

    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
};

// texture loader assumes all textures are named textureN, so those aliases to make it easier to read
#define DIFFUSE_MAP material.texture0
#define SPECULAR_MAP material.texture1

struct PointLight {
    vec3 position;
    vec3 color;
    
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

out vec4 fragmentColor;

in vec3 worldPosition;
in vec3 normal;
in vec2 uvCoords;

uniform Material material;
uniform PointLight pointLight;
uniform vec3 viewPos;

void main() {
    vec3 diffuseSample = vec3(texture(DIFFUSE_MAP, uvCoords));

    // calculates ambient
    vec3 ambient = material.ambientStrength * pointLight.color * diffuseSample;

    // calculates diffuse
    vec3 lightDir = normalize(pointLight.position - worldPosition);
    vec3 diffuse = max(dot(lightDir, normal), 0.0f) * material.diffuseStrength * pointLight.color * diffuseSample;

    // calculates specular
    vec3 lightDirReflected = reflect(-lightDir, normal);
    vec3 viewDir = normalize(viewPos - worldPosition);
    float specularIntensity = max(dot(lightDirReflected, viewDir), 0.0f);
    specularIntensity = pow(specularIntensity, material.shininess);
    vec3 specularSample = vec3(texture(DIFFUSE_MAP, uvCoords));
    vec3 specular = specularIntensity * material.specularStrength * pointLight.color * specularSample;

    // calculates final color
    fragmentColor = vec4(ambient + diffuse + specular, 1.0f);
}