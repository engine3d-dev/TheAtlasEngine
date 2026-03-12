#version 450

layout (location=0) in vec3 TexCoords; // Direction vector from vertex shader

layout (location=0) out vec4 out_Color;

layout(set = 0, binding = 1) uniform sampler2D cubeSampler;

// Constants to convert direction to spherical UVs
const vec2 invAtan = vec2(0.1591, 0.3183); // 1.0 / (2.0 * PI), 1.0 / PI

vec2 SampleEquirectangular(vec3 v) {
    // Normalize the input direction
    vec3 direction = normalize(v);
    
    // Calculate spherical coordinates
    // atan returns -PI to PI, asin returns -PI/2 to PI/2
    vec2 uv = vec2(atan(direction.z, direction.x), asin(direction.y));
    
    // Map to [0.0, 1.0] range
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    // Get the 2D UV coordinate from the 3D direction
    vec2 uv = SampleEquirectangular(TexCoords);
    
    // Sample the HDR map
    vec3 color = texture(cubeSampler, uv).rgb;
    
    // Simple Reinhard tone mapping (HDR values can be > 1.0, 
    // so we must compress them to [0, 1] for the screen)
    color = color / (color + vec3(1.0));
    
    // Gamma correction
    color = pow(color, vec3(1.0/2.2));
  
    out_Color = vec4(color, 1.0);
}