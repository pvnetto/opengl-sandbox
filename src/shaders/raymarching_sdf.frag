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
        
        // increments depth by distance, because it's guaranteed that
        // the geometry is at least "distance" away from the spherecast,
        // so there's no need to check smaller steps
        depth += dist;
        if(depth > MAX_DISTANCE) {
            return MAX_DISTANCE;
        }
    }
    
    return MAX_DISTANCE;
}

// Draw any sdf value below MAX_DISTANCE
float drawSDF(float sdf) {
    return 1. - step(MAX_DISTANCE - EPSILON, sdf);
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

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    uv = uv * 2. - 1.;
    uv.x *= (iResolution.x / iResolution.y);
    
    vec3 ro = vec3(0, 0., -5.); // Ray Origin/ Camera
    vec3 rd = getRayDirection(uv, ro, vec3(0.));

    float dist = getClosestIntersection(ro, rd);

    vec3 col = drawSDF(dist) * vec3(0.8, 0.2, 0.12);

    fragColor = vec4(col,1.0);
}