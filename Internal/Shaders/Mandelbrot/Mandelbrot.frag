#version 460 core

out vec4 FragColor;

// Vision and Calculation Uniforms
uniform vec2 u_Resolution;
uniform float u_Zoom;
uniform vec2 u_Position;
uniform float u_Rotation;
uniform int u_MaxIterations;
uniform float u_Bailout;
uniform float u_Power;
uniform int u_Algorithm; // 0: Mandelbrot, 1: Burning Ship, 2: Tricorn

// Julia Uniforms
uniform bool u_JuliaMode;
uniform vec2 u_JuliaC;

// Coloring Uniforms
uniform int u_ExteriorColoring; // 0: Step, 1: Smooth, 2: DistanceEstimation
uniform int u_InteriorColoring; // 0: Black, 1: White, 2: CustomColor
uniform vec3 u_InteriorColor;
uniform float u_ColorFrequency;
uniform float u_ColorOffset;
uniform bool u_OrbitColoring;
uniform float u_DistanceScale;

// Palette Uniforms
#define MAX_PALETTE_COLORS 16
uniform int u_ColorCount;
uniform vec3 u_Colors[MAX_PALETTE_COLORS];
uniform float u_ColorPositions[MAX_PALETTE_COLORS];

// Orbit Trap Uniforms
uniform int u_TrapType; // 0: None, 1: Point, 2: Circle, 3: Line, 4: Box, 5: Cross
uniform vec2 u_TrapP1;
uniform vec2 u_TrapP2;
uniform vec3 u_TrapColor;
uniform float u_TrapBlend;

// Complex power: z^p
vec2 CPow(vec2 z, float p) {
    float r = length(z);
    float a = atan(z.y, z.x);

    return pow(r, p) * vec2(cos(p * a), sin(p * a));
}

// Interpolates the color from the palette
vec3 GetPaletteColor(float t) {
    t = fract(t * u_ColorFrequency + u_ColorOffset);

    // If the color count is invalid, return an error color
    if (u_ColorCount < 2) return vec3(1.0, 0.0, 1.0);

    for (int i = 0; i < u_ColorCount - 1; i++) {
        if (t >= u_ColorPositions[i] && t <= u_ColorPositions[i + 1]) {
            float range = u_ColorPositions[i + 1] - u_ColorPositions[i];
            if (range == 0.0) return u_Colors[i];

            float localT = (t - u_ColorPositions[i]) / range;
            return mix(u_Colors[i], u_Colors[i + 1], localT);
        }
    }

    return u_Colors[u_ColorCount - 2];
}

// Distance from a point to a line segment
float DistanceToLine(vec2 p, vec2 a, vec2 b) {
    vec2 pa = p - a, ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);

    return length(pa - ba * h);
}

// Function to calculate the distance to a box (Chebyshev distance)
float DistanceToBox(vec2 p, vec2 center, vec2 size) {
    vec2 d = abs(p - center) - size;

    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

// Function to calculate the distance to a cross
float DistanceToCross(vec2 p, vec2 center) {
    vec2 d = abs(p - center);

    return min(d.x, d.y);
}

void main() {
    vec2 uv = (gl_FragCoord.xy * 2.0 - u_Resolution.xy) / u_Resolution.y;

    // Calculate the sine and cosine only once
    float cosR = cos(u_Rotation);
    float sinR = sin(u_Rotation);

    // Create a rotation matrix and apply it to the view coordinates
    mat2 rotationMatrix = mat2(cosR, -sinR, sinR, cosR);
    uv = rotationMatrix * uv;
    
    vec2 z, c;
    if (u_JuliaMode) {
        z = u_Position + uv / u_Zoom;
        c = u_JuliaC;
    } else {
        z = vec2(0.0);
        c = u_Position + uv / u_Zoom;
    }

    int i;
    vec2 dz = vec2(0.0); // Derivative for Distance Estimation
    if (u_JuliaMode) {
        dz = vec2(1.0, 0.0);
    }
    float minTrapDist = 1e20; // Minimum distance for Orbit Trap

    for (i = 0; i < u_MaxIterations; i++) {
        // The derivative is updated using the current 'z'
        if (u_ExteriorColoring == 2 || u_TrapType > 0) {
            // Avoid singularity at the origin for non-integer powers
            if (length(z) > 1e-6) {
                dz = u_Power * CPow(z, u_Power - 1.0) * dz;
            }
        }

        if (u_Algorithm == 1) { // Burning Ship
            z = vec2(abs(z.x), abs(z.y));
        } else if (u_Algorithm == 2) { // Tricorn
            z = vec2(z.x, -z.y); // Use the conjugate
        }

        // Z Update
        z = CPow(z, u_Power) + c;

        // For Mandelbrot, on the first iteration, dz must be 1
        if (!u_JuliaMode && i == 0) {
            dz = vec2(1.0, 0.0);
        }

        // Orbit Trap Logic
        if (u_TrapType > 0) {
            float dist = 1e20;

            if (u_TrapType == 1) { // Point
                dist = length(z - u_TrapP1);
            } else if (u_TrapType == 2) { // Circle
                dist = abs(length(z - u_TrapP1) - u_TrapP2.x);
            } else if (u_TrapType == 3) { // Line
                dist = DistanceToLine(z, u_TrapP1, u_TrapP2);
            } else if (u_TrapType == 4) { // Box
                dist = DistanceToBox(z, u_TrapP1, u_TrapP2);
            } else if (u_TrapType == 5) { // Cross
                dist = DistanceToCross(z, u_TrapP1);
            }

            minTrapDist = min(minTrapDist, dist);
        }

        if (dot(z, z) > u_Bailout) {
            break;
        }
    }

    vec3 finalColor;
    if (i >= u_MaxIterations) {
        // Interior Coloring
        if (u_InteriorColoring == 0) finalColor = vec3(0.0); // Black
        else if (u_InteriorColoring == 1) finalColor = vec3(1.0); // White
        else finalColor = u_InteriorColor; // CustomColor
    } else {
        // Exterior Coloring
        float t = 0.0;

        if (u_ExteriorColoring == 0) { // Step
            t = float(i) / float(u_MaxIterations);
        } else if (u_ExteriorColoring == 1) { // Smooth
            float log_zn = log(dot(z, z)) / 2.0;
            float nu = log(log_zn / log(2.0)) / log(2.0);
            t = (float(i) + 1.0 - nu) / float(u_MaxIterations);
        } else { // Distance Estimation
            // Calculate the squares of the magnitudes
            float z_sq = dot(z, z);
            float dz_sq = dot(dz, dz);

            // Security guards to prevent mathematical errors
            if (dz_sq < 1e-20 || z_sq < 1e-20) {
                // If in an unstable area, simply use Step mode as a fallback.
                t = float(i) / float(u_MaxIterations);
            } else {
                // If it is safe, calculate the distance
                float d = sqrt(z_sq / dz_sq) * log(z_sq) * 0.5; // 0.5 adjusts the scale
                t = d * u_DistanceScale;
            }
        }

        finalColor = GetPaletteColor(t);

        if (u_OrbitColoring) {
            // Use the angle of the end point 'z' to modify the color
            float angle = atan(z.y, z.x) / (2.0 * 3.14159265);
            vec3 orbit_color = GetPaletteColor(angle);
            // Mix the original color with the orbit color
            finalColor = mix(finalColor, orbit_color, 0.5);
        }
    }

    // Final mix with Orbit Trap
    if (u_TrapType > 0 && minTrapDist < 1e19) {
        float trapFactor = u_TrapBlend * exp(-2.0 * minTrapDist);
        finalColor = mix(finalColor, u_TrapColor, trapFactor);
    }

    FragColor = vec4(finalColor, 1.0);
}