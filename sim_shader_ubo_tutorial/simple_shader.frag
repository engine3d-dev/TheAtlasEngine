#version 450


layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;


layout(push_constant) uniform Push {
    mat4 Transform; // proj * view * model
    mat4 ModelMatrix;
} push;

void main(){
    // outColor = vec4(fragColor, 0.0);
    outColor = vec4(fragColor, 1.0);
}