#version 460

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec3 fragNormals;
layout(location = 2) in vec2 fragTexCoords;
layout(location = 3) in vec4 materialColor;
layout(location = 4) in vec3 FragPos;

layout(location = 0) out vec4 outColor;

struct directional_light {
    vec3 direction;
    vec3 view_position;
    vec4 ambient; // 4th channel represents the intensity
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
    vec4 ambient; // 4th channel is the intensity value
    vec4 diffuse;
    vec4 specular;
};

// we should only ever have a max of 1000 point lights in a given scene
const int max_point_lights = 1000;

layout(set = 0, binding = 1) uniform light_ubo {
    int num_lights;
    point_light sources[max_point_lights];
} light_src;


layout(set = 1, binding = 1) uniform sampler2D diffuse_texture;
layout(set = 1, binding = 2) uniform sampler2D specular_texture;

layout(set = 1, binding = 3) uniform material_ubo {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} material;

vec3 calc_dir_light(directional_light light, vec3 normal, vec3 view_dir) {
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
    vec3 ambient = ambient_offset * vec3(texture(diffuse_texture, fragTexCoords));
    vec3 diffuse = (diffuse_offset * light.color.rgb) * diff * vec3(texture(diffuse_texture, fragTexCoords));
    vec3 specular = specular_offset * spec * vec3(texture(specular_texture, fragTexCoords));

    return (ambient + diffuse + specular);
}

vec3 calc_point_light(point_light light, vec3 normal, vec3 fragPos, vec3 view_dir) {
    vec3 light_pos = vec3(light.position);
    vec3 dir_to_light = normalize(light_pos - fragPos);

    float diff = max(dot(normal, dir_to_light), 0.0);

    vec3 reflect_dir = reflect(-dir_to_light, normal);

    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    float dist = length(light_pos - fragPos);
    float attenuation = light.attenuation / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

    vec3 ambient = light.ambient.rgb * vec3(texture(diffuse_texture, fragTexCoords));
    vec3 diffuse =
      light.diffuse.xyz * diff * vec3(texture(diffuse_texture, fragTexCoords));
    vec3 specular = light.specular.xyz * spec * vec3(texture(specular_texture, fragTexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main() {

	// default ambience is used if there are no point lights assigned in a given scene
    vec3 default_ambience = (vec3(texture(diffuse_texture, fragTexCoords)) * fragColor.rgb) * 0.1;
    vec3 result = default_ambience;

	// we only want to select  the most minimal size of point lights.
	// if we have 2 point lights out of 1000, we iterate 2 point lights
	// if the cases that we try to have 1005 point lights, we only iterate the 1000'th point light
	// since that is how much we allocated to iterate that many point lights
	int num_point_lights = int(min(light_src.num_lights, max_point_lights));

	// iterate through all of the point lights in a given environment
    for (int i = 0; i < num_point_lights; i++) {
        point_light source_light = light_src.sources[i];
        vec3 view_pos = source_light.position.xyz;
        vec3 dir_to_light = normalize(view_pos - FragPos.xyz);
        result += calc_point_light(source_light, fragNormals, FragPos, dir_to_light) * (source_light.color.rgb * source_light.color.a);
    }

    outColor = vec4(result, 1.0);
}