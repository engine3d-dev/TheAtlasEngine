#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoords;
layout(location = 2) in vec3 fragNormals;
layout(location = 3) in flat int fragDiffuseIdx;
layout(location = 4) in flat int fragEnvironmentIdx;
// layout(location = 4) in flat int fragSpecularIdx;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D textures[];
layout(set = 0, binding = 2) uniform sampler2D environments[];

const vec2 inverse_atan = vec2(0.1591, 0.3183); // 1/(2*PI), 1/PI

// Convert 3D direction to 2D Equirectangular UV coordinates
vec2 sample_spherical_map(vec3 direction) {
    vec2 uv = vec2(atan(direction.z, direction.x), asin(direction.y));
    uv *= inverse_atan;
    uv += 0.5;
    return uv;
}

void main() {
    
    vec4 diffuse = texture(textures[nonuniformEXT(fragDiffuseIdx)], fragTexCoords);
    // vec4 specular = texture(textures[nonuniformEXT(fragSpecularIdx)], fragTexCoords);

    vec3 final_color = diffuse.rgb;
    final_color = final_color / (final_color + vec3(1.0));
    final_color = pow(final_color, vec3(1.0 / 2.2));
    outColor = vec4(final_color, 1.0);
    // vec3 material_color = vec3(diffuse.rgb * specular.rgb);
    // outColor = vec4(material_color, 1.0);
}