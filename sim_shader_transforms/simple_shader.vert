#version 450

//! @note Position and Color used in the Vertex class.
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push {
    mat4 transform;
    mat4 Projection;
    vec2 iResolution;
    vec3 color;
} push;

void main(){
    // gl_Position = vec4(Position, 0.0, 1.0);
    // mat2 p = (push.transform * 1.f);
    // p[0] *= (push.iResolution.y / push.iResolution.x);
    // p[1] *= (push.iResolution.y / push.iResolution.x);
    // gl_Position = vec4(p * Position + push.offset, 0.0, 1.0);
    // mat4 p = push.transform;
    // p[0] /= (push.iResolution.x / push.iResolution.y);
    // p[1] /= (push.iResolution.x / push.iResolution.y);
    // p[2] /= (push.iResolution.x / push.iResolution.y);
    // gl_Position = p * vec4(Position, 1.0);
    gl_Position = push.transform * vec4(Position, 1.0);
    fragColor = Color;
}

// layout(location = 0) in vec2 Position;
// layout(location = 1) in vec3 Color;

// // layout(location = 0) out vec3 fragColor;

// layout(push_constant) uniform Push {
//     mat2 transform;
//     vec2 offset;
//     vec2 iResolution;
//     vec3 color;
// } push;

// void main(){
//     // gl_Position = vec4(Position, 0.0, 1.0);
//     gl_Position = vec4(push.transform * Position + push.offset, 0.0, 1.0);
//     // fragColor = Color;
// }
