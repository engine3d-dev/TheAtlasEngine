#version 450


layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
    // vec3 DirectionToLight;
    mat4 ProjectionView;
    vec4 AmbientLightColor; // w = intensity
    vec3 LightPosition;
    vec4 LightColor; // w - is light intensity
} ubo;


layout(push_constant) uniform Push {
    mat4 Transform; // proj * view * model
    mat4 ModelMatrix;
    vec3 LightTransform;
} push;
void main(){
    // outColor = vec4(fragColor, 0.0);
    outColor = vec4(fragColor, 1.0);
}