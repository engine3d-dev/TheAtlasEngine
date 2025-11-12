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

// vec3 calc_point_light()


void main(){

    // This works! Need to get the TODO working!
    // float shininess = 64.0;
    
    
    // vec3 ambient_value = vec3(0.2, 0.2, 0.2);
    // vec3 diffuse_value = vec3(0.5, 0.5, 0.5);
    // vec3 specular_value = vec3(1.0, 1.0, 1.0);

    // TODO: Getting our uniforms specified in vk_renderer.cpp to apply these parameters here
    // vec3 ambient_value = material.ambient.rgb * material.ambient.a;
    // vec3 diffuse_value = material.diffuse.rgb;
    // vec3 specular_value = material.specular.rgb;
    // float shininess = material.shininess;

    // apply ambience
    /*
    vec3 ambient = ambient_value * texture(diffuse_texture, fragTexCoords).rgb;

    // apply diffuse
    vec3 norm = normalize(fragNormals);
    vec3 lightPos = vec3(0, 0, 0); // TODO: Replace this with a uniform that passes in the light position
    vec3 light_direction = normalize(lightPos - FragPos);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diffuse_value * diff * texture(diffuse_texture, fragTexCoords).rgb;

    // apply specular
    vec3 viewPos = vec3(0.0, 0.0, 0.0); // TODO: Replace this with a dynamic uniform
    vec3 view_direction = normalize(viewPos - FragPos);
    vec3 reflect_direction = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
    vec3 specular = specular_value * spec * texture(specular_texture, fragTexCoords).rgb;
    
    // vec4 lighting_result = vec4(ambient + diffuse + specular, 1.0);
    vec3 result = ambient + diffuse + specular;
    */
    

    // vec4 color = texture(diffuse_texture, fragTexCoords) * lighting_result; // working
    // vec4 color = final_texture * lighting_result; // working
    
    // do some camera and worldspace calculation
    

    // Adding point light code here

    // FragPos = position_to_world.xyz
    // fragNormal
    vec3 pos = light_src.light_source.position.xyz;
    // vec3 pos = vec3(-1.70, 5.58, 1.30);
    vec3 dir_to_light = pos - FragPos.xyz;
    float attenuation_offset = light_src.light_source.attenuation;
    float attenuation = attenuation_offset / dot(dir_to_light, dir_to_light);

    vec3 color = light_src.light_source.color.xyz * light_src.light_source.color.w;
    vec3 ambient_offset = light_src.light_source.ambient.rgb * light_src.light_source.ambient.a * attenuation;
    vec3 ambient = ambient_offset * vec3(texture(diffuse_texture, fragTexCoords));
    float diff = max(dot(fragNormals, normalize(dir_to_light)), 0.0);
    vec3 diffuse = light_src.light_source.diffuse.xyz * color * diff * vec3(texture(diffuse_texture, fragTexCoords));
    vec3 specular = light_src.light_source.specular.xyz * vec3(texture(specular_texture, fragTexCoords));



    // vec3 norm = normalize(fragNormals);
    // vec3 view_direction = normalize(dir_light_uniform.light_source.view_position - FragPos);
    // vec3 result = calc_dir_light(dir_light_uniform.light_source, view_direction, norm);

    // TEMPORARY: Adding this until point light work. Really, this should grab both diffuse and specular.
    // vec3 result = texture(diffuse_texture, fragTexCoords).rgb;
    // outColor = vec4(result, 1.0);
    vec3 result = (diffuse + ambient) * color;
    outColor = vec4(result, 1.0);
    // outColor = vec4(result, 1.0);
}