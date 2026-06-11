#version 450

#extension GL_EXT_nonuniform_qualifier : require
#extension GL_EXT_buffer_reference : require
#extension GL_EXT_scalar_block_layout : require

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoords;
layout(location = 2) in vec3 fragNormals;
layout(location = 3) in flat int fragDiffuseIdx;
layout(location = 4) in flat int fragSpecularIdx;
layout(location = 5) in vec3 FragPos;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D textures[];
layout(set = 0, binding = 2) uniform sampler2D environment_map; // used for IBL

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

layout(buffer_reference, scalar) buffer readonly SceneUniforms {
    mat4 view;
    mat4 proj;
};

const int max_point_lights = 1000;
layout(buffer_reference, scalar) buffer readonly PointLightsUniforms {
    int num_lights;
    int pad_0; // 4
    int pad_1; // 8
    int pad_2; // 12
    point_light point_lights[max_point_lights];
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
    PointLightsUniforms lights;
} push_const;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleEquirectangular(vec3 v) {
    vec3 direction = normalize(v);
    vec2 uv = vec2(atan(direction.z, direction.x), asin(direction.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

vec3 calc_point_light(point_light light, vec3 normal, vec3 fragPos, vec3 view_dir) {
    vec3 light_pos = vec3(light.position);
    vec3 dir_to_light = normalize(light_pos - fragPos);

    float diff = max(dot(normal, dir_to_light), 0.0);

    float material_shininess = 1.0f;

    vec3 reflect_dir = reflect(-dir_to_light, normal);

    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material_shininess);

    float dist = length(light_pos - fragPos);
    float attenuation = light.attenuation / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2));


    vec3 ambient = light.ambient.rgb * vec3(texture(textures[nonuniformEXT(fragDiffuseIdx)], fragTexCoords));
    // vec3 diffuse = light.diffuse.xyz * diff * vec3(texture(diffuse_texture, fragTexCoords));
    // vec3 specular = light.specular.xyz * spec * vec3(texture(specular_texture, fragTexCoords));
    vec3 diffuse = light.diffuse.xyz * diff * vec3(texture(textures[fragDiffuseIdx], fragTexCoords));
    vec3 specular = light.specular.xyz * spec * vec3(texture(textures[fragSpecularIdx], fragTexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main() {
    vec3 color = fragColor;
    vec4 diffuse = texture(textures[nonuniformEXT(fragDiffuseIdx)], fragTexCoords);
    vec4 specular = texture(textures[nonuniformEXT(fragSpecularIdx)], fragTexCoords);
    // vec3 default_ambience = (vec3(texture(diffuse_texture, fragTexCoords)) * fragColor.rgb) * 0.1;
    vec3 default_ambience = (vec3(texture(textures[nonuniformEXT(fragDiffuseIdx)], fragTexCoords)) * fragColor.rgb) * 0.1;
    vec3 final_color = default_ambience;

    PointLightsUniforms lights = push_const.lights;

    int num_point_lights = int(min(lights.num_lights, max_point_lights));

    for(int i = 0; i < num_point_lights; i++) {
        point_light src = lights.point_lights[i];
        vec3 view_pos = src.position.xyz;
        vec3 dir_to_light = normalize(view_pos - FragPos.xyz);
        final_color += calc_point_light(src, fragNormals, FragPos, dir_to_light) * (src.color.rgb * src.color.a);
    }

    // Uncomment to calculate the irradiance mapping
    // vec3 albedo = diffuse.rgb;
    // // Map the surface normal direction to the HDR map to get diffuse ambient light
    // vec3 N = normalize(fragNormals);
    // vec2 envUV = SampleEquirectangular(N);
    // // Low-frequency ambient lighting from the surrounding environment
    // // vec3 ambientIrradiance = texture(environment_map, envUV).rgb;
    // vec3 ambientIrradiance = textureLod(environment_map, envUV, 4.0).rgb;

    // // In a full PBR equation, this would be multiplied by your diffuse term (1.0 - metallic) * albedo
    // vec3 ambient = ambientIrradiance * albedo * 0.05; // Dim down slightly for a basic placeholder balance

    // // 4. Combine lighting components (Direct light + Ambient IBL)
    // vec3 final_color = albedo + ambient * color;


    outColor = vec4(final_color, 1.0);
}