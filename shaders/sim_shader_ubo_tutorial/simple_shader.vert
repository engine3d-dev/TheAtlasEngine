#version 450

//! @note Position and Color used in the Vertex class.
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec3 Normals;
layout(location = 3) in vec2 Uv;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;

layout(set = 0, binding = 0) uniform GlobalUbo {
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
    vec3 newPos = vec3(Position.x, Position.y, Position.z);
    gl_Position = push.Transform * vec4(newPos, 1.0);

    
    fragNormalWorld = normalize(mat3(push.ModelMatrix) * vec3(Normals.x, Normals.y, Normals.z));
    fragPosWorld = Position.xyz;
    fragColor = Color;

}