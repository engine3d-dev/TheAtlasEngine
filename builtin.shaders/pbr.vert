#version 450

#extension GL_EXT_buffer_reference : require
#extension GL_EXT_scalar_block_layout : require

/*

layout(location = 3) out MaterialData {
    int diffuse_idx;
    int specular_idx;
    int roughness_idx;
    int normal_idx;
    int parallax_idx;
} material_output;

*/

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoords;
layout(location = 3) in vec3 inNormals;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoords;
layout(location = 2) out vec3 fragNormals;
layout(location = 3) out flat int fragDiffuseIdx;
layout(location = 4) out flat int fragEnvironmentIdx;

layout(buffer_reference, scalar) buffer readonly SceneUniforms {
    mat4 view;
    mat4 proj;
    mat4 skybox_proj_view;
};

layout(buffer_reference, scalar) buffer ObjectsTable {
    mat4 model[];
};

layout(push_constant) uniform Constants {
    SceneUniforms global_ubo;
    ObjectsTable objects;
    int model_matrix_idx;
    int diffuse_idx;
} push_const;



void main() {
    SceneUniforms ubo = push_const.global_ubo;
    ObjectsTable object = push_const.objects;
    

    gl_Position = ubo.proj * ubo.view * object.model[push_const.model_matrix_idx] * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoords = inTexCoords;
    fragNormals = inNormals;
    fragDiffuseIdx = push_const.diffuse_idx;
    fragEnvironmentIdx = 0;
}