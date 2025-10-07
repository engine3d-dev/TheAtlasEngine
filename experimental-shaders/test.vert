#version 460

#extension GL_EXT_nonuniform_qualifier : enable // Required for descriptor indexing

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormals;
layout(location = 3) in vec2 inTexCoords;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec3 fragNormals;
layout(location = 2) out vec2 fragTexCoords;
layout(location = 3) out vec4 materialColor;

layout (set = 0, binding = 0) uniform UniformBuffer {
	mat4 MVP;
} ubo;

// To create a set=1, you just create another vk::descriptor_set object after the first descriptor set, because thats automatic.
layout (set = 1, binding = 0) uniform MaterialSource {
	mat4 model;
	vec4 color; // base color for object (NOTE: if materialColor uses this, we get error because we do not have a specific descriptor set for this layout yet, come back after getting food)
} material_src;

void main() {
	gl_Position = (ubo.MVP * material_src.model) * vec4(inPosition, 1.0);
	fragColor = vec4(inColor, 1.0);
	fragTexCoords = inTexCoords;
	fragNormals = inNormals;
	materialColor = material_src.color;
}