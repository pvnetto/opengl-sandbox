// View-space version of Phong shading model
#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec3 lightPos;

out vec4 fragmentColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
    // calculates ambient
    float ambientIntensity = 0.1f;
    vec3 ambient = ambientIntensity * lightColor;

    // calculates diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 diffuse = max(dot(normal, lightDir), 0.0f) * lightColor;

    // calculates specular
    vec3 viewDir = normalize(-fragPos);    // viewPos is the origin of view-space, so viewPos - fragPos = -fragPos
    vec3 lightDirReflected = normalize(reflect(-lightDir, normal));
    float shininess = 32;
    float specularIntensity = max(dot(viewDir, lightDirReflected), 0.0f);
    specularIntensity = pow(specularIntensity, shininess);
    vec3 specular = specularIntensity * lightColor;

    // calculates fragment color
    fragmentColor = vec4((ambient + diffuse + specular) * objectColor, 1.0f);
}
