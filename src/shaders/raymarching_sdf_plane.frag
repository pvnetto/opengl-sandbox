#define MAX_MARCH_DEPTH 100
#define MIN_DISTANCE 0.
#define MAX_DISTANCE 100.
#define EPSILON 0.0001

// SDF for an infinitely large plane, by IQ:
// https://iquilezles.org/www/articles/distfunctions/distfunctions.htm
float planeSDF(vec3 p, vec3 normal, float height) {
    return dot(p, normal) + height;
}

float sphereSDF(vec3 p, float r) {
    return length(p) - r;
}

float unionOP(float d1, float d2) {
    return min(d1, d2);
}

float sceneSDF(vec3 p) {
    float sp = sphereSDF(p, 1.);
    float pl = planeSDF(p, normalize(vec3(0., 1., 0.)), 1.);
    return unionOP(sp, pl);
}

float drawSDF(float dist) {
    return 1. - step(MAX_DISTANCE - EPSILON, dist);
}

float getClosestIntersection(vec3 ro, vec3 rd) {
    float depth = MIN_DISTANCE;
    for(int i = 0; i < MAX_MARCH_DEPTH; i++) {
        float dist = sceneSDF(ro + rd * depth);
        if(dist <= EPSILON) {
            return depth;
        }
        
        depth += dist;
        if(depth >= MAX_DISTANCE) {
            return MAX_DISTANCE;
        }
    }
    
    return MAX_DISTANCE;
}

vec3 getRayDirection(vec2 uv, vec3 ro, vec3 target) {
    vec3 forward = normalize(target - ro);
    vec3 right = normalize(cross(vec3(0., 1., 0.), forward));
    vec3 up = normalize(cross(forward, right));
    
    float fov = tan(radians(45.));
    return normalize(vec3(uv.x * fov * right + uv.y * fov * up + forward));
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

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    uv = uv * 2. - 1.;
    uv.x *= (iResolution.x / iResolution.y);
    
    vec3 ro = vec3(0., 2., -3.);
    vec3 rd = getRayDirection(uv, ro, vec3(0., 0., 0.));
    
    float dist = getClosestIntersection(ro, rd);
    
    vec3 p = ro + rd * dist;
    vec3 col = mix(vec3(0.6, 0.4, 0.8), 0.5 + estimateNormal(p) * 0.5, drawSDF(dist));

    fragColor = vec4(col,1.0);
}