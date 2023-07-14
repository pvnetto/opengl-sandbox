#define NEAR_PLANE 0.
#define FAR_PLANE 100.
#define EPSILON 0.001
#define MAX_MARCH_DEPTH 255

struct PointLight {
    vec3 pos;
    vec3 color;
};

PointLight l0 = PointLight(vec3(0., 16., 0.), vec3(1.));

float rand(vec2 p) {
    return fract(
        sin(
            dot(p, vec2(4984.54, 9874.23))
        ) * 49815.42
    );
}

float sSphere(vec3 p, float r) {
    return length(p) - r;
}

float sBox(vec3 p, vec3 bounds) {
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

float sPlane(vec3 p, vec3 normal, float height) {
    return dot(p, normal) + height;
}

float sBoxRounded(vec3 p, vec3 bounds, float iso) {
    return sBox(p, bounds) - iso;
}

float opUnion(float d1, float d2) {
    return min(d1, d2);
}

float opUnionSmooth(float d1, float d2, float maxDist) {
    float dist = abs(d1 - d2);
    float distNorm = max(maxDist - dist, 0.) / maxDist;
    
    const float isoMax = 0.25;
    return min(d1, d2) - distNorm * distNorm * maxDist * isoMax;
}

float opDifference(float d1, float d2) {
    return max(d1, -d2);
}

float sScene(vec3 p) {
    float anim = (cos(iTime) + 1.) * 4.;
    
    float res = 0.;
    float sp0 = sSphere(p + vec3(anim, 0., 0.), 1.3);
    float sp1 = sSphere(p + vec3(0., anim, 0.), 1.3);
    float sp2 = sSphere(p + vec3(0., 0., anim), 1.3);
    float sp3 = sSphere(p + vec3(0., -anim, 0.), 1.3);
    res = opUnionSmooth(sp0, sp1, 0.1);
    res = opUnionSmooth(res, sp2, 0.1);
    res = opUnionSmooth(res, sp3, 0.1);
    
    float bx0 = sBoxRounded(p, vec3(2.), 0.5);
    bx0 = opDifference(bx0, res - 1.05);
    res = opUnionSmooth(res, bx0, 0.1);
    
    float pln0 = sPlane(p, vec3(0., 1., 0.), 10.);
    res = opUnion(res, pln0);
    
    
    return res;
}

vec3 sNormal(vec3 p) {
    float d = sScene(p);
    vec2 eps = vec2(0.01, 0.);
    
    return normalize(
        d - vec3(
            sScene(p - eps.xyy),
            sScene(p - eps.yxy),
            sScene(p - eps.yyx)
        )
    );
}

float rRaymarch(vec3 ro, vec3 rd) {
    float depth = NEAR_PLANE;
    for(int i = 0; i < MAX_MARCH_DEPTH; i++) {
        float dist = sScene(ro + rd * depth);
        if(dist <= EPSILON)
            return depth;
        
        depth += dist;
    }
    
    return FAR_PLANE;
}

float rRaymarchClosestNorm(vec3 ro, vec3 rd, float weight) {
    float closest = 1.; 
    float currentRayLength = NEAR_PLANE;
    for(int i = 0; i < MAX_MARCH_DEPTH; i++) {
        float distanceFromScene = sScene(ro + rd * currentRayLength);
        if(distanceFromScene <= EPSILON)
            return 0.;  // returns 0 when ray hits an object
        
        // Dividing distance by ray length will result in smaller numbers
        // as ray length increases, avoiding the case where closest is the ray origin
        closest = min(closest, weight * distanceFromScene / currentRayLength);
        
        currentRayLength += distanceFromScene;
    }
    
    return closest;
}

vec3 rRayDirection(vec2 uv, vec3 ro, vec3 target) {
    vec3 forward = normalize(target - ro);
    vec3 right = normalize(cross(vec3(0., 1., 0.), forward));
    vec3 up = normalize(cross(forward, right));
    
    float fov = tan(radians(45.));
    
    return normalize(vec3(uv.x * fov * right + uv.y * fov * up + forward));
}

float dDraw(float dist) {
    return 1. - step(FAR_PLANE - EPSILON, dist);
}

// Shoots ray from object to light and checks for intersections.
// If intersecting, the object is shadowed, otherwise it's not.
float dShadow(vec3 ro, vec3 rd, float mint, float maxt) {
    return rRaymarch(ro, rd) < maxt ? 1. : 0.;
}

// Objects are shaded according to the smallest distance from its shadow ray
// and another object, so they're either shadowed (distance = 0), under the
// penumbra (distance close to 0), or not shadowed (distance close to 1) 
float dSoftShadow(vec3 ro, vec3 rd, float mint, float maxt, float k) {    
    return 1. - rRaymarchClosestNorm(ro, rd, k);
}

vec3 dLighting(vec3 p) {
    // point light with no attenuation
    vec3 l0Dir = normalize(p - l0.pos);
    vec3 normal = sNormal(p);
    
    vec3 ambient = 0.2 * l0.color;
    
    vec3 diffuse = max(dot(-l0Dir, normal), 0.) * l0.color;
    
    vec3 shadowDir = -l0Dir;
    vec3 shadowOrigin = p + normal * 0.01;
    float shadow = dSoftShadow(shadowOrigin, shadowDir, NEAR_PLANE, FAR_PLANE, 32.);
    
    vec3 color = (ambient + diffuse); 
    return mix(color, color * 0.3, shadow);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 uv = fragCoord/iResolution.xy;
    uv = uv * 2. - 1.;
    uv.x *= iResolution.x / iResolution.y;

    vec3 ro = vec3(cos(iTime) * 8.,5., sin(iTime) * 8.);
    vec3 rd = rRayDirection(uv, ro, vec3(0.));

    float dist = rRaymarch(ro, rd);
    vec3 p = ro + rd * dist;
    
    vec3 n = sNormal(p); 
    vec3 col = dLighting(p) * (n * 0.5 + 0.5);
    
    vec3 bg = mix(vec3(0.2, 0.2, 0.8), vec3(1.), uv.y);
    col = mix(bg, col, dDraw(dist));
    
    fragColor = vec4(col,1.0);
}