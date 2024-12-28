#version 450


layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;


// layout(push_constant) uniform Push {
//     mat4 Transform; // proj * view * model
//     mat4 ModelMatrix;
//     vec3 LightTransform;
// } push;
layout(push_constant) uniform Push {
    // mat4 Transform; // proj * view * model
    mat4 Projection;
    mat4 View;
    mat4 ModelMatrix;
    vec3 LightTransform;
} push;

void main(){
    // outColor = vec4(fragColor, 0.0);
    outColor = vec4(fragColor, 1.0);
}