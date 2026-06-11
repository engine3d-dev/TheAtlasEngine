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
layout(location = 4) out flat int fragSpecularIdx;
layout(location = 5) out vec3 FragPos;

// struct material {
//     int diffuse_idx;
//     int specular_idx;
// };

// layout(bufer_reference, scalar) buffer readonly MaterialUniforms {
//     material materials[];
// };

layout(buffer_reference, scalar) buffer readonly SceneUniforms {
    mat4 view;
    mat4 proj;
};

struct point_light {
    vec4 position;
    vec4 color;

    float attenuation;
    float constant;
    float linear;
    float quadratic;

    vec4 ambient; // 4th channel is the intensity value
    vec4 diffuse;
    vec4 specular;
};

const int max_point_lights = 1000;
layout(buffer_reference, scalar) buffer readonly PointLightsUniforms {
    int num_lights;
    int pad_0; // 4
    int pad_1; // 8
    int pad_2; // 12
    point_light light_sources[max_point_lights];
};

layout(buffer_reference, scalar) buffer ObjectsTable {
    mat4 model[];
};

layout(push_constant) uniform Constants {
    SceneUniforms global_ubo;
    ObjectsTable objects;
    int model_matrix_idx;
    int diffuse_idx;
    int specular_idx;
    PointLightsUniforms point_lights;
} push_const;



void main() {
    SceneUniforms ubo = push_const.global_ubo;
    ObjectsTable object = push_const.objects;
    
    vec4 position_in_world = object.model[push_const.model_matrix_idx] * vec4(inPosition, 1.0);
    // gl_Position = ubo.proj * ubo.view * object.model[push_const.model_matrix_idx] * vec4(inPosition, 1.0);
    gl_Position = ubo.proj * ubo.view * position_in_world;
    fragColor = inColor;
    fragTexCoords = inTexCoords;
    fragNormals = inNormals;
    fragDiffuseIdx = push_const.diffuse_idx;
    fragSpecularIdx = push_const.specular_idx;
    FragPos = position_in_world.xyz;
}