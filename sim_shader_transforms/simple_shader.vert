#version 450

//! @note Position and Color used in the Vertex class.
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec3 Normals;
layout(location = 3) in vec2 Uv;


layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push {
    mat4 Transform; // proj * view * model
    mat4 ModelMatrix;
    vec3 LightTransform;
} push;

// const vec3 dir_to_light = normalize(vec3(1.0, -3.0, -1.0));
// vec3 light_transform = vec3(1.0, -3.0, -1.0);
// const vec3 dir_to_light = normalize(vec3(1.0, -3.0, -1.0));
// vec3 dir_to_light = normalize(push.LightTransform);
// const vec3 dir_to_light = normalize();
vec3 dir_to_light = normalize(push.LightTransform);

void main(){
    // vec4 worldPositionSpace = push.ModelMatrix * vec3(Position, 1.0);
    gl_Position = push.Transform * vec4(Position, 1.0);

    // mat3 normal_mat = transpose(inverse(mat3(push.ModelMatrix)));
    vec3 normalize_world_space = normalize(mat3(push.ModelMatrix) * Normals);
    float LightIntensity = max(dot(normalize_world_space, dir_to_light), 0) / 1.5f;

    // fragColor = Color;
    fragColor = LightIntensity * Color;
}