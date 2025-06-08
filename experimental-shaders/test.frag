#version 460

layout (binding = 1) uniform sampler2D texSampler;
layout (location = 0) in vec4 fragColor;
layout (location = 1) in vec3 fragNormals;
layout (location = 2) in vec2 fragTexCoords;

layout(location = 0) out vec4 outColor;

//aaaaa
void main()
{
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    outColor = texture(texSampler, fragTexCoords) * color;
}