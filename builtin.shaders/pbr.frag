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
    vec4 camera_pos;
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
    vec4 diffuse = texture(textures[nonuniformEXT(fragDiffuseIdx)], fragTexCoords);
    vec4 specular = texture(textures[nonuniformEXT(fragSpecularIdx)], fragTexCoords);

    // vec3 albedo = diffuse.rgb;
    vec3 albedo = fragColor.rgb;
    vec3 final_color = (albedo * fragColor.rgb) * 0.1;

    PointLightsUniforms lights = push_const.lights;

    int num_point_lights = int(min(lights.num_lights, max_point_lights));

    for(int i = 0; i < num_point_lights; i++) {
        point_light src = lights.point_lights[i];
        vec3 view_pos = src.position.xyz;
        vec3 dir_to_light = normalize(view_pos - FragPos.xyz);
        final_color += calc_point_light(src, fragNormals, FragPos, dir_to_light) * (src.color.rgb * src.color.a);
    }

    SceneUniforms ubo = push_const.global_ubo;

    // vec3 view_pos = vec3(inverse(ubo.view)[3]);
    // vec3 view_dir = normalize(ubo.camera_pos.xyz - FragPos);
    vec3 normal = normalize(fragNormals);
    vec3 Incident = normalize(FragPos - ubo.camera_pos.xyz);



    // Calculating diffuse term
    vec2 diffuse_uv = SampleEquirectangular(normal);
    // vec3 ambient_irradiance = textureLod(environment_map, normal, 5.0).rgb;
    vec3 ambient_irradiance = textureLod(environment_map, diffuse_uv, 5.0).rgb;
    vec3 ibl_diffuse = ambient_irradiance * albedo * 0.5;

    // Calculating the specular term
    vec3 reflect_dir = reflect(Incident, normal);
    vec2 specular_uv = SampleEquirectangular(reflect_dir);
    // vec3 ambient_radiance = textureLod(environment_map, reflect_dir, 1.5).rgb;
    vec3 ambient_radiance = textureLod(environment_map, specular_uv, 1.5).rgb;


    vec3 ibl_specular = ambient_radiance * specular.rgb * 0.5;

    final_color += (ibl_diffuse + ibl_specular);

    outColor = vec4(final_color, 1.0);
}