#version 460

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec3 fragNormals;
layout(location = 2) in vec2 fragTexCoords;
layout(location = 3) in vec4 materialColor;
layout(location = 4) in vec3 FragPos;

layout(location = 0) out vec4 outColor;

// #extension GL_EXT_debug_printf : enable

struct directional_light {
    vec3 direction;
    vec3 view_position; // TODO: Might Remove
    // 4th channel represents the intensity
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 color;
};

struct point_light {
    vec4 position;
    vec4 color;
    float attenuation;
    float constant;
    float linear;
    float quadratic;
    // 4th channel is the intensity value
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

// we should only ever have a max of 1000 point lights in a given scene
const int max_point_lights = 1000;

// layout(set = 0, binding = 1) uniform light_ubo {
//     point_light source;
// } light_src;

layout(set = 0, binding = 1) uniform light_ubo {
    int num_lights;
    point_light sources[10];
} light_src;

/*

void main() {
    vec3 result = ...;
    for(int i = 0; i < scene_ubo.num_point_lights; i++) {
        point_light source = scene_ubo.sources[i];

        result += calc_point_light(source, normal);
    }
}

*/

/*

struct scene_light_ubo {
    // 1.) this will allow for getting size of light sources
    // and how many light sources to send to the shader
    // 2.) then we will query all game objects with a point light and modify
those here std::span<point_light> point_light_sources;
};

// setting binding = 1 to represent a given array of point lights that are
created in a given scene layout(set = 0, binding = 1) uniform light_ubo { int
num_of_point_lights; point_light sources[num_of_point_lights]; }
scene_point_lights;

*/

layout(set = 1, binding = 1) uniform sampler2D diffuse_texture;
layout(set = 1, binding = 2) uniform sampler2D specular_texture;

layout(set = 1, binding = 3) uniform material_ubo {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} material;

// TODO: Implement directional lighting at a later time
vec3
calc_dir_light(directional_light light, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);

    // specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // final result computation
    // because of alignment we set the ambient, diffuse, specular to vec3 and
    // use the 4th channel as the intensity values at least for ambient strength
    vec3 ambient_offset = light.ambient.rgb * light.ambient.a;
    vec3 diffuse_offset = light.diffuse.rgb * light.diffuse.a;
    vec3 specular_offset = light.specular.rgb;
    vec3 ambient =
      ambient_offset * vec3(texture(diffuse_texture, fragTexCoords));
    vec3 diffuse = (diffuse_offset * light.color.rgb) * diff *
                   vec3(texture(diffuse_texture, fragTexCoords));
    vec3 specular =
      specular_offset * spec * vec3(texture(specular_texture, fragTexCoords));

    return (ambient + diffuse + specular);
}

vec3
calc_point_light(point_light light, vec3 normal, vec3 fragPos, vec3 view_dir) {
    vec3 light_pos = vec3(light.position);
    vec3 dir_to_light = normalize(light_pos - fragPos);

    float diff = max(dot(normal, dir_to_light), 0.0);

    vec3 reflect_dir = reflect(-dir_to_light, normal);

    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    float dist = length(light_pos - fragPos);
    float attenuation =
      light.attenuation /
      (light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

    // vec3 color = light.color.rgb * light.color.a;
    // vec3 ambient_offset = light.ambient.rgb * light.ambient.a;
    vec3 ambient =
      light.ambient.rgb * vec3(texture(diffuse_texture, fragTexCoords));
    vec3 diffuse =
      light.diffuse.xyz * diff * vec3(texture(diffuse_texture, fragTexCoords));
    vec3 specular = light.specular.xyz * spec *
                    vec3(texture(specular_texture, fragTexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

/*
// TODO: Implement shadow calculations at later time...
float shadow_calculation(vec3 fragPosLightSpace) {
    vec4 pos = vec4(fragPosLightSpace, 1.0);
}
*/

void
main() {
    // Adding point light code here

    // extracting specific the point light from our objects and updating via the
    // uniforms here point_light light = light_src.source;

    // FragPos = position_to_world.xyz
    // fragNormal
    /*
    vec3 view_pos = light.position.xyz;
    // vec3 pos = vec3(-1.70, 5.58, 1.30);
    vec3 dir_to_light = view_pos - FragPos.xyz;
    float attenuation_offset = light.attenuation;
    float attenuation = attenuation_offset / dot(dir_to_light, dir_to_light);

    vec3 color = light.color.rgb * light.color.a;
    vec3 ambient_offset = light.ambient.rgb * light.ambient.a * attenuation;
    vec3 ambient = ambient_offset * vec3(texture(diffuse_texture,
    fragTexCoords)); float diff = max(dot(fragNormals, normalize(dir_to_light)),
    0.0); vec3 diffuse = light.diffuse.xyz * diff *
    vec3(texture(diffuse_texture, fragTexCoords)); vec3 specular =
    light.specular.xyz * vec3(texture(specular_texture, fragTexCoords));

    // Applied blinn-phongs term (blinn-phong's shading)
    vec3 viewPos = light.position;
    vec3 view_direction = normalize(viewPos - FragPos);
    vec3 half_angle = normalize(dir_to_light + view_direction);
    float blinn_term = dot(fragNormals, half_angle);
    blinn_term = clamp(blinn_term, 0, 1);
    blinn_term = pow(blinn_term, 32.0);
    specular += color.xyz * attenuation * blinn_term;
    */

    // TODO: Implement shadow calculation with the given lighting system
    // implementation float shadow_bias = shadow_calculation(light.position);

    // vec3 result = (ambient + diffuse + specular) * color;

    // vec3 view_pos = light.position.xyz;
    // vec3 dir_to_light = normalize(view_pos - vec3(FragPos));
    // vec3 result = calc_point_light(light, fragNormals, FragPos,
    // dir_to_light);

    // vec3 result = vec3(0.5);z

    // point_light source_light = light_src.sources[0];
    // vec3 view_pos = source_light.position.xyz;
    // vec3 dir_to_light = normalize(view_pos - vec3(FragPos));
    // vec3 result = calc_point_light(source_light, fragNormals, FragPos,
    // dir_to_light);

    // vec3 result = vec3(0.6);

    // if no point lights are appointed to this. Then we are going to just by
    // default show the mesh without applied meshes
    vec3 default_ambience =
      (vec3(texture(diffuse_texture, fragTexCoords)) * fragColor.rgb) * 0.1;
    vec3 result = default_ambience;

    for (int i = 0; i < light_src.num_lights; i++) {
        point_light source_light = light_src.sources[i];
        vec3 view_pos = source_light.position.xyz;
        vec3 dir_to_light = normalize(view_pos - FragPos.xyz);
        result +=
          calc_point_light(source_light, fragNormals, FragPos, dir_to_light) * (source_light.color.rgb * source_light.color.a);
    }

    // point_light source_light = light_src.sources[0];
    // vec3 view_pos = source_light.position.xyz;
    // vec3 dir_to_light = normalize(view_pos - FragPos.xyz);
    // result += calc_point_light(source_light, fragNormals, FragPos,
    // dir_to_light);

    outColor = vec4(result, 1.0);
}