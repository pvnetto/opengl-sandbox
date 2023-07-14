// Introducing some space modifying functions from https://www.youtube.com/watch?v=s8nFqwOho-s
// For more, refer to: http://mercury.sexy/hg_sdf/

#define PI 3.14159265

#define MAX_MARCH_DEPTH 100
#define MIN_DISTANCE 0.
#define MAX_DISTANCE 100.
#define EPSILON 0.0001

float planeSDF(vec3 p, vec3 normal, float height) {
    return dot(p, normal) + height;
}

float sphereSDF(vec3 p, float r) {
    return length(p) - r;
}

float unionOP(float d1, float d2) {
    return min(d1, d2);
}

float spTiling1(float axis, float v) {
    return mod(axis + v, v * 2.) - v;
}

void spMirror2(inout vec2 p, vec2 offset) {
    // abs mirrors the coordinate and offset moves it,
    // so mirrored coordinates don't overlap
    p = abs(p) - offset;
    p = vec2(max(p.x, p.y), min(p.x, p.y));
}

float sceneSDF(vec3 p) {
    //p.x = spTiling1(p.x, 3.);
    //p.y = spTiling1(p.y, 3.);
    //p.z = spTiling1(p.z, 4.);
    spMirror2(p.xz, vec2(2., 2.));
    float sp = sphereSDF(p, 1.);
    return sp;
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
    
    vec3 ro = vec3(6., 2., 6.);
    vec3 rd = getRayDirection(uv, ro, vec3(0., 0., 0.));
    
    float dist = getClosestIntersection(ro, rd);
    
    vec3 p = ro + rd * dist;
    vec3 col = mix(vec3(0.6, 0.4, 0.8), 0.5 + estimateNormal(p) * 0.5, drawSDF(dist));

    fragColor = vec4(col,1.0);
}