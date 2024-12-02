#version 450

// //! @note Position and Color used in the Vertex class.
// layout(location = 0) in vec3 Position;
// layout(location = 1) in vec3 Color;
// layout(location = 2) in vec3 Normals;
// layout(location = 3) in vec2 Uv;


// layout(location = 0) out vec3 fragColor;

// layout(set = 0, binding = 0) uniform GlobalUbo {
//     vec3 DirectionToLight;
// } ubo;

// layout(push_constant) uniform Push {
//     mat4 Transform; // proj * view * model
//     mat4 ModelMatrix;
// } push;

// const float AMBIENT = 0.02;

// void main(){
//     vec3 newPos = vec3(Position.x,Position.y-.5f,Position.z);
//     gl_Position = push.Transform * vec4(newPos,1.0);

//     vec3 NormalWorldSpace = normalize(mat3(push.ModelMatrix) * Normals);
//     float LightIntensity = max(dot(NormalWorldSpace, normalize(ubo.DirectionToLight)), 0) / 1.5f;
//     fragColor = LightIntensity * Color;
// }

//! @note Position and Color used in the Vertex class.
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec3 Normals;
layout(location = 3) in vec2 Uv;


layout(location = 0) out vec3 fragColor;

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

// const vec3 dir_to_light = normalize(vec3(1.0, -3.0, -1.0));
// vec3 dir_to_light = normalize(push.LightTransform);
// vec3 dir_to_light = normalize(ubo.DirectionToLight);

// NEW: 

void main(){
    vec3 newPos = vec3(Position.x, Position.y, Position.z);
    vec3 dir_to_light = ubo.LightPosition;
    // vec4 positionWorld = push.ModelMatrix * vec4(Position, 1.0);
    
    // vec4 worldPositionSpace = push.ModelMatrix * vec4(newPos, 1.0);
    gl_Position = push.Transform * vec4(newPos,1.0);
    // gl_Position = push.Transform * positionWorld;

    // mat3 normal_mat = transpose(inverse(mat3(push.ModelMatrix)));
    vec3 normalize_world_space = normalize(mat3(push.ModelMatrix) * vec3(Normals.x, Normals.y, Normals.z));

    // vec3 DirectionToLight = ubo.LightPosition - positionWorld.xyz;
    // vec3 DirectionToLight = ubo.LightPosition - newPos.xyz;
    vec3 DirectionToLight = dir_to_light - newPos.xyz;
    float attenuation = 1.0 / dot(DirectionToLight, DirectionToLight);

    vec3 LightColor = ubo.LightColor.xyz * ubo.LightColor.w;
    vec3 AmbientLight = ubo.AmbientLightColor.xyz * ubo.AmbientLightColor.w * attenuation;
    vec3 DiffuseLighting = LightColor * max(dot(normalize_world_space, normalize(DirectionToLight)), 0);

    // float LightIntensity = max(dot(normalize_world_space, dir_to_light), 0) / 1.5f;

    // fragColor = Color;
    // fragColor = LightIntensity * Color;

    fragColor = (DiffuseLighting + AmbientLight) * Color;
}