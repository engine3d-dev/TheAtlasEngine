#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormals;
layout(location = 3) in vec2 inTexCoords;

layout (location=0) out vec3 TexCoords;

layout (set = 0, binding = 0) readonly uniform UniformBuffer {
	mat4 view_proj;
} ubo;

void main() {
	TexCoords = inPosition;
	vec4 pos = ubo.view_proj * vec4(inPosition, 1.0);
	gl_Position = pos.xyww;
}