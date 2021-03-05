// Ray Marching SDF

#define MAX_MARCH_DEPTH 100
#define MIN_DISTANCE 0.
#define MAX_DISTANCE 100.
#define EPSILON 0.00001

float sphereSDF(vec3 p, float rad) {
    return length(p) - rad;
}

float sceneSDF(vec3 p) {
    return sphereSDF(p, 1.);
}

float getClosestIntersection(vec3 ro, vec3 rd) {
    float depth = MIN_DISTANCE;
    for(int i = 0; i < MAX_MARCH_DEPTH; i++) {
        float dist = sceneSDF(ro + rd * depth);
        if(dist < EPSILON) {
            return depth;
        }
        
        depth += dist;
        if(depth > MAX_DISTANCE) {
            return MAX_DISTANCE;
        }
    }
    
    return MAX_DISTANCE;
}

float drawSDF(float sdf) {
    return 1. - step(MAX_DISTANCE - EPSILON, sdf);
}

vec3 estimateNormal(vec3 p) {
    float d = sceneSDF(p);
    vec2 eps = vec2(0.01, 0.);
    
    return normalize(
        d - vec3(
            sceneSDF(p - eps.xyy),
            sceneSDF(p - eps.yxy),
            sceneSDF(p - eps.yyx)
        )
    );
}

vec3 lighting(vec3 p, vec3 viewPos) {
    vec3 lightDir = normalize(vec3(1., -1., 1.));
    vec3 lightColor = vec3(1., 1., 1.);
    vec3 normal = estimateNormal(p);
    
    vec3 ambient = 0.1 * lightColor;
    
    vec3 diffuse = max(dot(-lightDir, normal), 0.) * lightColor;
    
    vec3 lightDirRef = reflect(lightDir, normal);
    vec3 viewDir = normalize(viewPos - p);
    float specularIntensity = max(dot(lightDirRef, viewDir), 0.);
    float shininess = 32.;
    specularIntensity = pow(specularIntensity, shininess);
    vec3 specular = specularIntensity * lightColor;
    
    return (ambient + diffuse + specular);
}

// Creates a look-at matrix, just like GLM's
mat4 lookAt(vec3 p, vec3 target) {
    vec3 worldUp = vec3(0., 1., 0.);
    
    vec3 forward = normalize(target - p);
    vec3 left = normalize(cross(forward, worldUp));
    vec3 up = normalize(cross(left, forward));
    
    return mat4(
        vec4(-left, 0.),
        vec4(up, 0.),
        vec4(forward, 0.),
        vec4(0., 0., 0., 1.)
    );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float fov = 45.;
    vec2 uv = (fragCoord.xy - 0.5 * iResolution.xy);
    uv /= iResolution.y / tan(radians(fov) * 0.5);
    
    vec3 ro = vec3(0, 12., -5.);
    vec3 rd = normalize(vec3(uv.x, uv.y, 1.));
    
    // Transforms ray direction so it points towards target position
    mat4 lookAtMat = lookAt(ro, vec3(0., 0., 0.));
    rd = vec3(lookAtMat * vec4(rd, 0.));
    
    float dist = getClosestIntersection(ro, rd);
    
    vec3 p = ro + rd * dist;
    vec3 col = drawSDF(dist) * lighting(p, ro) * vec3(1., 0.4, 0.2);

    fragColor = vec4(col,1.0);
}