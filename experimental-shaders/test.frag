#version 460

// layout (binding = 1) uniform sampler2D texSampler;
layout (location = 0) in vec4 fragColor;
layout (location = 1) in vec3 fragNormals;
layout (location = 2) in vec2 fragTexCoords;
layout (location = 3) in vec4 materialColor;
// layout (location = 3) in vec4 in_fragColor;

layout(location = 0) out vec4 outColor;

layout (set = 1, binding = 1) uniform sampler2D texture1;
layout (set = 1, binding = 2) uniform sampler2D texture2;

void main(){
    // vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    // outColor = texture(texSampler, fragTexCoords) * color;
    // vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    // vec4 color = texture(texture1, fragTexCoords) + texture(texture2, fragTexCoords) * materialColor; // not working
    vec4 color = texture(texture1, fragTexCoords) * materialColor; // working
    // vec4 color = in_fragColor;
    outColor = color;
    // outColor = texture(texSampler, fragTexCoords) * color;
}