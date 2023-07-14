#version 330 core

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;

    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;

    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;

    float cutoffAngle;          // angles are given in radians
    float shadowSmoothAngle;    // applies smooth shadows to fragments between [cutoff - smooth, cutoff] angles
};

in vec3 worldPosition;
in vec3 normal;
in vec2 uvCoords;

out vec4 fragmentColor;

uniform Material material;
uniform SpotLight light;
uniform vec3 viewPos;

void main() {
    vec3 diffuseSample = vec3(texture(material.diffuseMap, uvCoords));

    float distanceFromLight = length(light.position - worldPosition);

    // calculates ambient
    vec3 ambient = material.ambientStrength * diffuseSample * light.color;

    // calculates diffuse
    vec3 lightDir = normalize(worldPosition - light.position);
    vec3 diffuse = max(dot(-lightDir, normal), 0.0f) * diffuseSample * light.color;

    // calculates specular
    vec3 lightDirReflected = reflect(lightDir, normal);
    vec3 viewDir = normalize(viewPos - worldPosition);
    float specularIntensity = max(dot(lightDirReflected, viewDir), 0.0f);
    specularIntensity = pow(specularIntensity, material.shininess);
    vec3 specularSample = vec3(texture(material.specularMap, uvCoords));
    vec3 specular = specularIntensity * material.specularStrength * specularSample * light.color;

    // checks if fragment is inside spotlight
    float angleBetweenLight = acos(dot(lightDir, light.direction));
    float spotlightIntensity = angleBetweenLight > light.cutoffAngle ? 0.f : 1.f;

    // applies smooth shadows around spotlight edges
    float outerRingStartAngle = light.cutoffAngle - light.shadowSmoothAngle;
    float smoothIntensity = 1.0f - smoothstep(outerRingStartAngle, light.cutoffAngle, angleBetweenLight);
    spotlightIntensity *= smoothIntensity;

    // applies attenuation
    // attenuation = 1 / kc + kl*d + kq*d^2
    float attenuation = 1.0f / (
        light.constantAttenuation
        + light.linearAttenuation * distanceFromLight
        + light.quadraticAttenuation * pow(distanceFromLight, 2));
    attenuation *= spotlightIntensity;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // calculates fragment color
    fragmentColor = vec4(ambient + diffuse + specular, 1.0f); 
}