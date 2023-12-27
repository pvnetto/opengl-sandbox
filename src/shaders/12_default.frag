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

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} inFragment;

out vec4 fragmentColor;

uniform Material material;
uniform PointLight pointLight;
uniform vec3 viewPos;

void main() {
    vec3 diffuseSample = vec3(texture(material.diffuseMap, inFragment.uv));

    // calculates ambient
    vec3 ambient = material.ambientStrength * pointLight.color * diffuseSample;

    // calculates diffuse
    vec3 lightDir = normalize(pointLight.position - inFragment.worldPosition);
    vec3 diffuse = max(dot(lightDir, inFragment.normal), 0.0f) * material.diffuseStrength * pointLight.color * diffuseSample;

    // calculates specular
    vec3 lightDirReflected = reflect(-lightDir, inFragment.normal);
    vec3 viewDir = normalize(viewPos - inFragment.worldPosition);
    float specularIntensity = max(dot(lightDirReflected, viewDir), 0.0f);
    specularIntensity = pow(specularIntensity, material.shininess);
    vec3 specularSample = vec3(texture(material.diffuseMap, inFragment.uv));
    vec3 specular = specularIntensity * material.specularStrength * pointLight.color * specularSample;

    // calculates final color
    fragmentColor = vec4(ambient + diffuse + specular, 1.0f);
}