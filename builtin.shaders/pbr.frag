#version 450

#extension GL_EXT_nonuniform_qualifier : require
#extension GL_EXT_buffer_reference : require
#extension GL_EXT_scalar_block_layout : require

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoords;
layout(location = 2) in vec3 fragNormals;
layout(location = 3) in flat int fragDiffuseIdx;
layout(location = 4) in flat int fragSpecularIdx;
layout(location = 5) in vec3 FragPos; // contains world position

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

    // float material_shininess = 1.0f;
    float roughness = 0.5f;
    float material_shininess = mix(128.0, 1.0, roughness);

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



const float PI = 3.14159265359;
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}


void main() {
    SceneUniforms ubo = push_const.global_ubo;


    vec4 diffuse_texture = texture(textures[nonuniformEXT(fragDiffuseIdx)], fragTexCoords);
    vec4 specular_texture = texture(textures[nonuniformEXT(fragSpecularIdx)], fragTexCoords);
    vec3 albedo = (fragColor.rgb * diffuse_texture.rgb);
    vec3 final_color = (albedo * diffuse_texture.rgb) * 0.1;

    float roughness = 0.2;
    float metallic = 0.2;
    float ao = 0.5;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // vec3 view_pos = vec3(inverse(ubo.view)[3]);
    // vec3 view_dir = normalize(ubo.camera_pos.xyz - FragPos);

    // N
    vec3 normal = normalize(fragNormals);

    // V
    vec3 view_dir = normalize(ubo.camera_pos.xyz - FragPos);

    // vec3 albedo = diffuse.rgb;
    // vec3 albedo = fragColor.rgb;
    // vec3 final_color = (albedo * diffuse.rgb) * 0.1;

    PointLightsUniforms lights = push_const.lights;

    int num_point_lights = int(min(lights.num_lights, max_point_lights));

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < num_point_lights; i++) {
        point_light src = lights.point_lights[i];

        // L
        vec3 dir_to_light = normalize(src.position.xyz - FragPos);
        vec3 H = normalize(dir_to_light + view_dir);
        float dist = length(src.position.xyz - FragPos);
        // float attenuation = 1.0 / (dist * dist);
        float attenuation = src.attenuation / (src.constant + src.linear * dist + src.quadratic * pow(dist, 2));
        vec3 radiance = src.color.rgb * src.color.a * attenuation;

        // cook-torrance brdf
        float NDF = DistributionGGX(normal, H, roughness);
        float G = GeometrySmith(normal, view_dir, dir_to_light, roughness);
        vec3 F = fresnelSchlick(max(dot(H, view_dir) , 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, view_dir), 0.0) * max(dot(normal, dir_to_light), 0.0) + 0.0001;
        vec3 specular = numerator / denominator * specular_texture.rgb;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(normal, dir_to_light), 0.0);

        Lo += calc_point_light(src, fragNormals, FragPos, dir_to_light) * (src.color.rgb * src.color.a);
        // vec3 direct_ambient = src.ambient.rgb * albedo * attenuation;
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    float max_texture_mip_levels = 0.0;
    vec3 kS = fresnelSchlick(max(dot(normal, view_dir), 0.0), F0);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
    vec2 environment_uv = SampleEquirectangular(normal);
    // vec3 irradiance = texture(environment_map, environment_uv).rgb;
    vec3 irradiance = textureLod(environment_map, environment_uv, max_texture_mip_levels).rgb;
    vec3 diffuse = irradiance * albedo;
    vec3 ambient = (kD * diffuse.rgb) * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    outColor = vec4(color, 1.0);
}