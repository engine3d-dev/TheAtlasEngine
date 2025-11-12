#version 460

layout (location = 0) in vec4 fragColor;
layout (location = 1) in vec3 fragNormals;
layout (location = 2) in vec2 fragTexCoords;
layout (location = 3) in vec4 materialColor;
layout(location = 4) in vec3 FragPos;

layout(location = 0) out vec4 outColor;

layout (set = 1, binding = 1) uniform sampler2D diffuse_texture;
layout(set = 1, binding = 2) uniform sampler2D specular_texture;

layout(set = 1, binding = 3) uniform material_ubo {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} material;

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
    vec3 position;
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

layout(set = 0, binding = 1) uniform light_ubo {
    point_light light_source;
} light_src;

vec3 calc_dir_light(directional_light light, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);

    // specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // final result computation
    // because of alignment we set the ambient, diffuse, specular to vec3 and use the 4th channel as the intensity values
    // at least for ambient strength
    vec3 ambient_offset = light.ambient.rgb * light.ambient.a;
    vec3 diffuse_offset = light.diffuse.rgb * light.diffuse.a;
    vec3 specular_offset = light.specular.rgb;
    vec3 ambient = ambient_offset * vec3(texture(diffuse_texture, fragTexCoords));
    vec3 diffuse = (diffuse_offset * light.color.rgb) * diff * vec3(texture(diffuse_texture, fragTexCoords));
    vec3 specular = specular_offset * spec * vec3(texture(specular_texture, fragTexCoords));
    
    return (ambient + diffuse + specular);
}

/*
float shadow_calculation(vec3 fragPosLightSpace) {
    vec4 pos = vec4(fragPosLightSpace, 1.0);
}
*/

void main(){
    // Adding point light code here

    // extracting specific the point light from our objects and updating via the uniforms here
    point_light light = light_src.light_source;

    // FragPos = position_to_world.xyz
    // fragNormal
    vec3 pos = light.position.xyz;
    // vec3 pos = vec3(-1.70, 5.58, 1.30);
    vec3 dir_to_light = pos - FragPos.xyz;
    float attenuation_offset = light.attenuation;
    float attenuation = attenuation_offset / dot(dir_to_light, dir_to_light);

    vec3 color = light.color.rgb * light.color.a;
    vec3 ambient_offset = light.ambient.rgb * light.ambient.a * attenuation;
    vec3 ambient = ambient_offset * vec3(texture(diffuse_texture, fragTexCoords));
    float diff = max(dot(fragNormals, normalize(dir_to_light)), 0.0);
    vec3 diffuse = light.diffuse.xyz * diff * vec3(texture(diffuse_texture, fragTexCoords));
    vec3 specular = light.specular.xyz * vec3(texture(specular_texture, fragTexCoords));

    // Applied blinn-phongs term (blinn-phong's shading)
    vec3 viewPos = light.position;
    vec3 view_direction = normalize(viewPos - FragPos);
    vec3 half_angle = normalize(dir_to_light + view_direction);
    float blinn_term = dot(fragNormals, half_angle);
    blinn_term = clamp(blinn_term, 0, 1);
    blinn_term = pow(blinn_term, 32.0);
    specular += color.xyz * attenuation * blinn_term;

    // TODO: Implement shadow calculation with the given lighting system implementation
    // float shadow_bias = shadow_calculation(light.position);

    vec3 result = (diffuse + ambient) * color;

    outColor = vec4(result, 1.0);
}