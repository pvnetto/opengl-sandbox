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

struct PointLight {
    vec3 position;
    vec3 color;

    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;

    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;

    float cutoffAngle;          // given in radians
    float shadowSmoothAngle;    // given in radians
};

in VertexShaderOutput {
    vec3 worldPosition;
    vec2 uv;
    vec3 normal;
} inFragment;

out vec4 fragmentColor;

#define NUM_LIGHTS 4
uniform Material material;
uniform DirectionalLight directionalLights[NUM_LIGHTS];
uniform int enabledDirectionalLights;
uniform PointLight pointLight;
uniform SpotLight spotlight;
uniform vec3 viewPos;

vec3 CalculateDirectionalLight(DirectionalLight light, Material material, vec3 viewPos) {
    vec3 diffuseSample = vec3(texture(material.diffuseMap, inFragment.uv));

    // calculates ambient
    vec3 ambient = material.ambientStrength * light.color * diffuseSample;

    // calculates diffuse
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = max(dot(lightDir, inFragment.normal), 0.0f) * material.diffuseStrength * light.color * diffuseSample;

    // calculates specular
    vec3 lightDirReflected = reflect(-lightDir, inFragment.normal);
    vec3 viewDir = normalize(viewPos - inFragment.worldPosition);
    float specularIntensity = max(dot(lightDirReflected, viewDir), 0.0f);
    specularIntensity = pow(specularIntensity, material.shininess);
    vec3 specularSample = vec3(texture(material.specularMap, inFragment.uv));
    vec3 specular = specularIntensity * material.specularStrength * light.color * specularSample;

    // calculates final light color
    return (ambient + diffuse + specular); 
}

vec3 CalculatePointLight(PointLight light, Material material, vec3 viewPos) {
    vec3 diffuseSample = vec3(texture(material.diffuseMap, inFragment.uv));

    // calculates ambient
    vec3 ambient = material.ambientStrength * light.color * diffuseSample;

    // calculates diffuse
    vec3 lightDir = normalize(light.position - inFragment.worldPosition);
    vec3 diffuse = max(dot(lightDir, inFragment.normal), 0.0f) * material.diffuseStrength * light.color * diffuseSample;

    // calculates specular
    vec3 lightDirReflected = reflect(-lightDir, inFragment.normal);
    vec3 viewDir = normalize(viewPos - inFragment.worldPosition);
    float specularIntensity = max(dot(lightDirReflected, viewDir), 0.0f);
    specularIntensity = pow(specularIntensity, material.shininess);
    vec3 specularSample = vec3(texture(material.specularMap, inFragment.uv));
    vec3 specular = specularIntensity * material.specularStrength * light.color * specularSample;

    // calculates attenuation
    float distanceFromFragment = length(light.position - inFragment.worldPosition);
    float attenuation = 1.0f / (
        light.constantAttenuation +
        light.linearAttenuation * distanceFromFragment +
        light.quadraticAttenuation * pow(distanceFromFragment, 2)
    );

    // calculates final light color
    return (ambient + diffuse + specular) * attenuation; 
}

vec3 CalculateSpotLight(SpotLight light, Material material, vec3 viewPos) {
    vec3 diffuseSample = vec3(texture(material.diffuseMap, inFragment.uv));

    // calculates ambient
    vec3 ambient = material.ambientStrength * light.color * diffuseSample;

    // calculates diffuse
    vec3 lightDir = normalize(inFragment.worldPosition - light.position);
    vec3 diffuse = max(dot(-lightDir, inFragment.normal), 0.0f) * material.diffuseStrength * light.color * diffuseSample;

    // calculates specular
    vec3 lightDirReflected = reflect(lightDir, inFragment.normal);
    vec3 viewDir = normalize(viewPos - inFragment.worldPosition);
    float specularIntensity = max(dot(lightDirReflected, viewDir), 0.0f);
    specularIntensity = pow(specularIntensity, material.shininess);
    vec3 specularSample = vec3(texture(material.specularMap, inFragment.uv));
    vec3 specular = specularIntensity * material.specularStrength * light.color * specularSample;

    // calculates cutoff
    float lightFragAngle = acos(dot(lightDir, light.direction));
    float spotlightIntensity = lightFragAngle < light.cutoffAngle ? 1.0f : 0.0f;
    float outerRingStart = light.cutoffAngle - light.shadowSmoothAngle;
    spotlightIntensity *= (1.0f - smoothstep(outerRingStart, light.cutoffAngle, lightFragAngle));    // applies shadow smoothing around edges

    // calculates attenuation
    float distanceFromFragment = length(light.position - inFragment.worldPosition);
    float attenuation = 1.0f / (
        light.constantAttenuation +
        light.linearAttenuation * distanceFromFragment +
        light.quadraticAttenuation * pow(distanceFromFragment, 2)
    );

    ambient *= attenuation * spotlightIntensity;
    diffuse *= attenuation * spotlightIntensity;
    specular *= attenuation * spotlightIntensity;

    // calculates final light color
    return (ambient + diffuse + specular); 
}

void main() {
    vec3 result = vec3(0.0);

    for(int i = 0; i < enabledDirectionalLights; i++) {
        result += CalculateDirectionalLight(directionalLights[i], material, viewPos);
    }

    result += CalculatePointLight(pointLight, material, viewPos);
    result += CalculateSpotLight(spotlight, material, viewPos);

    fragmentColor = vec4(result, 1.0f);
}