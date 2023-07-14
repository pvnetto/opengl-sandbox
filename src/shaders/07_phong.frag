#version 330 core

in vec3 worldPosition;
in vec3 normal;

out vec4 fragmentColor;

uniform vec3 objectColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() {
    // calculates ambient
    float ambientIntensity = 0.1f;
    vec3 ambient = ambientIntensity * lightColor;

    // calculates diffuse
    vec3 lightDir = normalize(lightPos - worldPosition);
    vec3 diffuse = max(dot(lightDir, normal), 0.0f) * lightColor;

    // calculates specular
    vec3 viewDir = normalize(viewPos - worldPosition);
    vec3 lightDirReflected = reflect(-lightDir, normal);
    
    float shininess = 64;
    float specularIntensity = max(dot(lightDirReflected, viewDir), 0.0f);
    specularIntensity = pow(specularIntensity, shininess);
    vec3 specular = specularIntensity * lightColor;

    // combines all components
    fragmentColor = vec4((ambient + diffuse + specular) * objectColor, 1.0f);
}