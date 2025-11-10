#version 460

layout (location = 0) in vec4 fragColor;
layout (location = 1) in vec3 fragNormals;
layout (location = 2) in vec2 fragTexCoords;
layout (location = 3) in vec4 materialColor;

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
};

layout(set = 1, binding = 4) uniform directional_light_ubo {
    directional_light light_source;
} dir_light_uniform;

layout(location = 4) in vec3 FragPos;

/*
vec3 calc_dir_light(directional_light p_light, vec3 p_normal, vec3 p_view_direction) {
    vec3 light_dir = normalize(-p_light.direction);
    float diff = max(dot(p_normal, light_dir), 0.0);
    vec3 reflect_dir = reflect(-light_dir, p_normal);
    float spec = pow(max(dot(p_view_direction, reflect_dir), 0.0), material.shininess);

    // combine the results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, fragTexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse.rgb, fragTexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular.rgb, fragTexCoords));
    return (ambient + diffuse + specular);
}
*/


void main(){

    // This works! Need to get the TODO working!
    // float shininess = 64.0;
    
    
    // vec3 ambient_value = vec3(0.2, 0.2, 0.2);
    // vec3 diffuse_value = vec3(0.5, 0.5, 0.5);
    // vec3 specular_value = vec3(1.0, 1.0, 1.0);

    // TODO: Getting our uniforms specified in vk_renderer.cpp to apply these parameters here
    vec3 ambient_value = material.ambient.rgb * material.ambient.a;
    vec3 diffuse_value = material.diffuse.rgb;
    vec3 specular_value = material.specular.rgb;
    float shininess = material.shininess;

    // apply ambience
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
    

    // vec4 color = texture(diffuse_texture, fragTexCoords) * lighting_result; // working
    // vec4 color = final_texture * lighting_result; // working

    /*
    vec3 norm = normalize(fragNormals);
    vec3 view_direction = normalize(dir_light_uniform.view_position - FragPos);
    vec3 result = calc_dir_light(dir_light_uniform.light_source, view_direction, norm);
    */

    outColor = vec4(result, 1.0);
}