#define EPSILON 0.01
#define MIN_DISTANCE 0.
#define MAX_DISTANCE 100.
#define MAX_MARCH_DEPTH 100

float sphereSDF(vec3 p, float r) {
    return length(p) - r;
}

float boxSDF(vec3 p, vec3 bounds) {
    vec3 pabs = abs(p);
    
    float dx = pabs.x - bounds.x;
    float dy = pabs.y - bounds.y;
    float dz = pabs.z - bounds.z;
    
    return sqrt(
        pow(max(dx, 0.), 2.) +
        pow(max(dy, 0.), 2.) +
        pow(max(dz, 0.), 2.)
    );
}

float roundedBoxSDF(vec3 p, vec3 bounds, float iso) {
    return boxSDF(p, bounds) - iso;
}

float unionOP(float d1, float d2) {
    return min(d1, d2);
}

// Inspired by (Media Molecule's polynomial smin implementation):
// https://www.iquilezles.org/www/articles/smin/smin.htm
// Returns min(d1, d2) when points are further than 'maxDist' away from each other,
// otherwise it returns a smooth value between both distances
float sminOP(float d1, float d2, float maxDist) {
    float dist = abs(d1 - d2);                              // distance between points
    float distNorm = max(maxDist - dist, 0.0) / maxDist;    // normalized distance
    
    // Values greater than 0.25 will break smoothing, as it will move to an isoline that
    // is too further from the original SDF
    const float isoMax = 0.25;
    
    return min(d1, d2) - distNorm * distNorm * maxDist * isoMax;
}

// Same as smin but cubic, so it has C2 continuity instead of C1 
float sminCubicOP(float d1, float d2, float maxDist) {
    float dist = abs(d1 - d2);
    float distNorm = max(maxDist - dist, 0.0) / maxDist;
    
    // Iso values should be smaller for cubic smin
    const float isoMax = 1. / 6.;
    return min(d1, d2) - distNorm * distNorm * distNorm * maxDist * isoMax;
}

float sceneSDF(vec3 p) {
    float sp1 = sphereSDF(p + vec3(0., abs(cos(iTime)) * -3., 0.), 1.);
    float sp2 = sphereSDF(p + vec3(-1.5, 0., 0.), 1.);
    float bx = roundedBoxSDF(p, vec3(1.), 0.3);
    
    float pct = unionOP(sp1, sp2);
    pct = sminOP(pct, bx, 0.1);
    
    return pct;
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
    return normalize(uv.x * fov * right + uv.y * fov * up + forward);
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
    
    vec3 ro = vec3(2., 2., 4.);
    vec3 rd = getRayDirection(uv, ro, vec3(0.));
    
    float dist = getClosestIntersection(ro, rd);
    vec3 p = ro + rd * dist;
    
    vec3 col = drawSDF(dist) * (0.5 + estimateNormal(p) * 0.5);

    fragColor = vec4(col,1.0);
}