#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormals;
layout(location = 3) in vec2 inTexCoords;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec3 fragNormals;
layout(location = 2) out vec2 fragTexCoords;

layout (binding = 0) uniform UniformBuffer {
	mat4 MVP;
} ubo;

void main() {
	gl_Position = ubo.MVP * vec4(inPosition, 1.0);
	fragColor = vec4(inColor, 1.0);
	fragTexCoords = inTexCoords;
	fragNormals = inNormals;
}