#define PI 3.14159265

// I/O lookup table
// number_of_sides = 3
// side_angle = 360 / number_of_sides = 120

// modulates angle from -60 to 60 (-side_angle / 2, side_angle / 2)
// pixel_angle      modulated_angle     is breakpoint?      cos(modulated_angle)
// 180          	-60 .. 60   	    < side 1            1 / 2
// 210          	30                                      sqrt(3) / 2
// 240          	0                                       1
// 270          	-30                                     sqrt(3) / 2
// 300          	-60 .. 60   	    < side 2            1 / 2
// 330          	30
// 360          	0                                       1
// 0            	0	
// 30           	-30
// 60           	-60 .. 60		    < side 3            1 / 2
// 90           	30	
// 120          	0                                       1
// 150          	-30


// the fact that this shader always uses an angle that ranges
// from [-side_angle / 2, side_angle / 2] is what makes it draw
// the wanted shape, because it creates a distance field that
// uses cossine values to determine distance from center 

#define PI 3.14159625

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // normalizes pixel coordinates
    vec2 uv = fragCoord/iResolution.xy;
    uv = uv * 2.0 - vec2(1.0);
    uv.x *= (iResolution.x / iResolution.y);

    vec2 p = uv;
    
    // calculates polar coordinates
    float polarAng = atan(p.x, p.y) + PI;
    float polarLength = length(p);
    
    // calculates polar coordinates for any vertex of chosen shape
    int numSides = 3;
    float baseAngle = (2. * PI) / float(numSides);
    
    // calculates modulated polar coordinates
    float currentSide = polarAng / baseAngle;
    currentSide = floor(0.5 + currentSide);
    float polarAngMod = currentSide * baseAngle;
    polarAngMod -= polarAng;   // range: [-baseAngle / 2, baseAngle / 2]
    
    // cos is equal for every pixel at a given angle from center,
    // but it's multiplied by distance from center, so pixels
    // further away have stronger color
    float pct = cos(polarAngMod) * polarLength;
    vec3 col = vec3(pct);

    fragColor = vec4(col,1.0);
}