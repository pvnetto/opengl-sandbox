// Emission maps will output colors regardless of light conditions,
// that is, they don't need to be lit to output a color
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

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} inFragment;

out vec4 fragmentColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform float time;

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

    // calculates emission
    int emissionStrength = specularSample.r == 0 && specularSample.g == 0 && specularSample.b == 0 ? 1 : 0;     // Checks if texture is inside black part of specular map
    vec3 emissionSample = vec3(texture(material.emissionMap, inFragment.uv + vec2(0, time)));
    vec3 emission = emissionSample * emissionStrength;
    
    // calculates fragment color
    fragmentColor = vec4((ambient + diffuse + specular + emission), 1.0f);
}