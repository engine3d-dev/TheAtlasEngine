#version 460

layout (location = 0) in vec4 fragColor;
layout (location = 1) in vec3 fragNormals;
layout (location = 2) in vec2 fragTexCoords;
layout (location = 3) in vec4 materialColor;

layout(location = 0) out vec4 outColor;

layout (set = 1, binding = 1) uniform sampler2D diffuse_texture;
layout(set = 1, binding = 2) uniform sampler2D specular_texture;

// struct material {
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
//     float shininess;
// };

layout(set = 1, binding = 3) uniform material_ubo {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

struct point_light {
	vec3 pos;
	vec4 color;
	float attenuation;
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout(set = 1, binding = 2) uniform LightingSource {
	point_light source; // single point light source
} lighting_src;

// in vec3 FragPos;
layout(location = 4) in vec3 FragPos;


void main(){

    float shininess = 64.0;
    vec3 ambient_value = vec3(0.2, 0.2, 0.2);
    vec3 diffuse_value = vec3(0.5, 0.5, 0.5);
    vec3 specular_value = vec3(1.0, 1.0, 1.0);

    // float shininess = material.shininess;
    // vec3 ambient_value = material.ambient;
    // vec3 diffuse_value = material.diffuse;
    // vec3 specular_value = material.specular;

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
    vec4 color = vec4(result, 1.0);
    outColor = color;
}