// Ray Marching SDF
#define MAX_MARCH_DEPTH 100
#define MIN_DISTANCE 0.
#define MAX_DISTANCE 100.
#define EPSILON 0.0001

float sphereSDF(vec3 p, float r) {
    return length(p) - r;
}

float sceneSDF(vec3 p) {
    return sphereSDF(p, 1.);
}

float getClosestIntersection(vec3 ro, vec3 rd) {
    float depth = MIN_DISTANCE;
    
    for(int i = 0; i < MAX_MARCH_DEPTH; i++) {
        float dist = sceneSDF(ro + rd * depth);
        if(dist <= EPSILON){
            return depth;
        }
        
        depth += dist;
        if(depth >= MAX_DISTANCE) {
            return MAX_DISTANCE;
        }
    }

    return MAX_DISTANCE;
}

float drawSDF(float dist) {
    return 1. - step(MAX_DISTANCE - EPSILON, dist);
}

vec3 getRayDirection(vec2 uv, vec3 ro, vec3 target) {
    // calculates camera's orthonormal basis
    vec3 forward = normalize(target - ro);
    vec3 right = normalize(cross(vec3(0., 1., 0.), forward));
    vec3 up = normalize(cross(forward, right));
    
    // scales vector (x, y) using field of view, so high fov values
    // makes the ray go "outside" the screen, while lower fov values
    // will steer the ray forward
    float fov = tan(radians(45.));
    return normalize(uv.x * fov * right + uv.y * fov * up + forward);
}

// estimates normal using derivatives
// don't know exactly how this works yet, but it's like
// calculating the direction between the center of the
// sdf and a point
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
    vec3 lightDir = normalize(vec3(0.5, -1., 0.3));
    vec3 lightColor = vec3(1., 0.8, 1.);
    vec3 normal = estimateNormal(p);
    
    vec3 ambient = 0.1 * lightColor;
    
    vec3 diffuse = max(dot(-lightDir, normal), 0.) * lightColor;
    
    vec3 lightDirRefl = reflect(lightDir, normal);
    vec3 viewDir = normalize(p - viewPos);
    float specularIntensity = max(dot(lightDirRefl, -viewDir), 0.);
    float shininess = 32.;
    specularIntensity = pow(specularIntensity, shininess);
    vec3 specular = specularIntensity * lightColor;
    
    return (ambient + diffuse + specular);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    uv = uv * 2. - 1.;
    uv.x *= (iResolution.x / iResolution.y);
    
    vec3 ro = vec3(0., 0., -2.);
    vec3 rd = getRayDirection(uv, ro, vec3(0.));
    
    float dist = getClosestIntersection(ro, rd);

    vec3 p = ro + rd * dist;
    vec3 col = drawSDF(dist) * lighting(p, ro) * vec3(0.2, 0.8, 0.1);

    fragColor = vec4(col,1.0);
}