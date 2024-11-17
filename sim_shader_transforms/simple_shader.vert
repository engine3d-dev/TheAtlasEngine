#version 450

layout(location = 0) in vec2 Position;
layout(location = 1) in vec3 Color;

// layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push {
    mat2 transform;
    vec2 offset;
    vec3 color;
} push;

void main(){
    // gl_Position = vec4(Position, 0.0, 1.0);
    gl_Position = vec4(push.transform * Position + push.offset, 0.0, 1.0);
    // fragColor = Color;
}