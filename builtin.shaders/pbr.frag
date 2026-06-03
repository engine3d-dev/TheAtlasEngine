#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoords;
layout(location = 2) in vec3 fragNormals;
layout(location = 3) in flat int fragDiffuseIdx;
// layout(location = 4) in flat int fragSpecularIdx;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D textures[];

void main() {
    
    // vec4 diffuse = texture(textures[nonuniformEXT(fragDiffuseIdx)], fragTexCoords);
    // vec4 specular = texture(textures[nonuniformEXT(fragSpecularIdx)], fragTexCoords);

    outColor = texture(textures[nonuniformEXT(fragDiffuseIdx)], fragTexCoords);
    // vec3 material_color = vec3(diffuse.rgb * specular.rgb);
    // outColor = vec4(material_color, 1.0);
}