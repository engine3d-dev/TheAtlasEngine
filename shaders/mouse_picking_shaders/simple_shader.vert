#version 450

//! @note Position and Color used in the vertex class.
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec3 Normals;
layout(location = 3) in vec2 Uv;


layout(location = 0) out vec3 fragColor;

// we set our descriptor set to binding = 0
// meaning that our descriptor set references to GlobalUbo
// layout(set = 0, binding = 0) uniform GlobalLightUbo {
//     mat4 Model; // light model
//     vec3 Direction; // light dir
// } light_ubo;

layout(set = 0, binding = 0) uniform GlobalCameraUbo{
    mat4 Projection;
    mat4 View;
    mat4 Model;
    vec3 LightTransform;
} camera_ubo;

layout(push_constant) uniform Push {
    mat4 Projection;
    mat4 View;
    mat4 Model;
    vec3 LightTransform;
    vec4 Color;
    vec2 MousePosition;
} push;



vec3 dir_to_light = normalize(push.LightTransform);
const float AMBIENT = 0.2;

/*
void main(){
    gl_Position = (push.Projection * push.View * push.Model) * vec4(Position, 1.0);

    vec3 normalize_world_space = normalize(mat3(push.Model) * Normals);
    float LightIntensity = AMBIENT + max(dot(normalize_world_space, dir_to_light), 0);

    vec3 baseColor = LightIntensity * Color * push.Color.xyz;

    // VERY BASIC "PICKING" (Highly Inaccurate)
    vec4 screenPos = gl_Position;
    // vec4 screenPos = vec4(Position, 1.0);
    screenPos.xyz /= screenPos.w; // Perspective divide

    vec2 fragScreenPos = screenPos.xy * 0.5 + 0.5; // NDC to screen space (0-1)

    float dist = length(fragScreenPos - push.MousePosition); // dist in screen space

    float threshold = 0.5; // Adjust this (very sensitive)

    if (dist < threshold) {
        fragColor = baseColor;
    } else {
        // fragColor = baseColor;
        fragColor = vec3(1.0, 0.0, 0.0); // Red highlight (or any effect you want)
    }
}
*/


void main(){
    gl_Position = (push.Projection * push.View * push.Model) * vec4(Position, 1.0);

    vec3 normalize_world_space = normalize(mat3(push.Model) * Normals);

    float LightIntensity = AMBIENT + max(dot(normalize_world_space, dir_to_light), 0);

    fragColor = LightIntensity * Color * push.Color.xyz;
}
