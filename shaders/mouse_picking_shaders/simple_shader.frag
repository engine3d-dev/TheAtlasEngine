#version 450


layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;


// layout(push_constant) uniform Push {
//     // mat4 Transform; // proj * view * model
//     mat4 Projection;
//     mat4 View;
//     mat4 Model;
//     vec3 LightTransform;
// } push;

// layout(push_constant) uniform Push {
//     // mat4 Transform; // proj * view
//     mat4 Projection;
//     mat4 View;
//     mat4 Model;
//     vec3 LightTransform;
// } push;
// we set our descriptor set to binding = 0
// meaning that our descriptor set references to GlobalUbo
// layout(set=  0, binding = 0) uniform GlobalLightUbo {
//     mat4 Model; // light model
//     vec3 Direction; // light dir
// } light_ubo;

layout(set = 0, binding = 0) uniform GlobalCameraUbo{
    mat4 Projection;
    mat4 View;
    mat4 Model;
    vec3 LightTransform;
    // vec4 Color;
} camera_ubo;

layout(push_constant) uniform Push {
    mat4 Projection;
    mat4 View;
    mat4 Model;
    vec3 LightTransform;
    vec4 Color;
    vec2 MousePosition;
} push;

void main(){
    outColor = vec4(fragColor, 1.0);
}