#version 450

#extension GL_EXT_buffer_reference : require

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoords;
layout(location = 3) in vec3 inNormals;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoords;
layout(location = 2) out vec3 fragNormals;
layout(location = 3) out flat int fragTexIndex;

layout(buffer_reference, std140) buffer readonly UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
};

layout(push_constant) uniform Constants {
    int texture_index;
    UniformBufferObject global_ubo_address;
} push_const;



void main() {
    UniformBufferObject ubo = push_const.global_ubo_address;

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoords = inTexCoords;
    fragNormals = inNormals;
    fragTexIndex = push_const.texture_index;
}