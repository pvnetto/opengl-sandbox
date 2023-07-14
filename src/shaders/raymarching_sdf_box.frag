#define EPSILON 0.001
#define MIN_DISTANCE 0.
#define MAX_DISTANCE 100.
#define MAX_MARCH_DEPTH 100

// Inspired by: https://www.youtube.com/watch?v=62-pRVZuS5c
float boxSDF(vec3 p, vec3 halfBounds) {
    // Moves coordinate to the top-right quadrant of the box
    vec3 pabs = abs(p);
    
    // Calculates distance between each axis of point and top-right quadrant 
    float dx = pabs.x - halfBounds.x;
    float dy = pabs.y - halfBounds.y;
    float dz = pabs.z - halfBounds.z;
    
    // Whenever a point P is aligned with the quadrant Q on an axis A, its distance
    // can be calculated as pA - qA, except for diagonals where all axis should be
    // considered. this is further explained in the video
    
    // Generalizes formula for distance between point and box corner
    // by canceling all axis whose values are smaller than 0, because
    // this means that the point is aligned with the quadrant on an axis
    return sqrt(
        pow(max(dx, 0.), 2.) +
        pow(max(dy, 0.), 2.) +
        pow(max(dz, 0.), 2.)
    );
}

// Inspired by: https://www.youtube.com/watch?v=s5NGeUV2EyU
float roundedBoxSDF(vec3 p, vec3 halfBounds, float iso) {    
    // Moves box to a diferent isoline, resulting in round edges.
    // This technique can be used for any shape, as isolines are inherent
    // to all distance fields. The SDF of a sphere can be thought of as
    // moving a point to a different isoline, thus making it rounder and bigger.
    return boxSDF(p, halfBounds) - iso;
}

float sceneSDF(vec3 p) {
    return roundedBoxSDF(p, vec3(1., 3., 2.), 1.);
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
    uv.x *= iResolution.x / iResolution.y;
    
    vec3 ro = vec3(cos(iTime) * 6., cos(iTime) * 2., sin(iTime) * 6.);
    vec3 rd = getRayDirection(uv, ro, vec3(0.));
    
    float dist = getClosestIntersection(ro, rd);
    
    vec3 p = ro + rd * dist;
    vec3 col = drawSDF(dist) * (0.5 + estimateNormal(p) * 0.5);

    fragColor = vec4(col, 1.0);
}