#version 450

//! @note Position and Color used in the Vertex class.
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec3 Normals;
layout(location = 3) in vec2 Uv;


layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
    vec3 DirectionToLight;
} ubo;

layout(push_constant) uniform Push {
    mat4 Transform; // proj * view * model
    mat4 ModelMatrix;
} push;

const float AMBIENT = 0.02;

void main(){

    // gl_Position = push.Transform * vec4(Position, 1.0);
    vec3 newPos = vec3(Position.x,Position.y-.5f,Position.z);
    // vec4 worldPositionSpace = push.ModelMatrix * vec4(newPos, 1.0);
    gl_Position = push.Transform * vec4(newPos,1.0);

    vec3 NormalWorldSpace = normalize(mat3(push.ModelMatrix) * Normals);
    float LightIntensity = AMBIENT + max(dot(NormalWorldSpace, normalize(ubo.DirectionToLight)), 0);
    fragColor = LightIntensity * Color;
}