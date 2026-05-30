#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoords;
layout(location = 2) in vec3 fragNormals;
layout(location = 3) in flat int fragTexIndex;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D textures[];

void main() {
    
    outColor = texture(textures[nonuniformEXT(fragTexIndex)], fragTexCoords);
}