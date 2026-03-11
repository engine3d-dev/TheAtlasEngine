#version 450

layout (location=0) in vec3 TexCoords;

layout (location=0) out vec4 out_Color;

layout(set = 0, binding = 1) uniform samplerCube cubeSampler;

void main() {
	out_Color = texture(cubeSampler, TexCoords);
}