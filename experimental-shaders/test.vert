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
layout(location = 4) out vec3 FragPos;

layout (set = 0, binding = 0) uniform UniformBuffer {
	mat4 proj_view;
} ubo;

// To create a set=1, you just create another vk::descriptor_set object after the first descriptor set, because thats automatic.
layout (set = 1, binding = 0) uniform geometry_ubo {
	mat4 model;
	vec4 color; // base color for object (NOTE: if materialColor uses this, we get error because we do not have a specific descriptor set for this layout yet, come back after getting food)
} geometry_src;

void main() {
	// FragPos = material_src.model * vec4(inPositoin, 1.0);
	vec4 pos_to_world = geometry_src.model * vec4(inPosition, 1.0);

	// vec3 normalize_mat = normalize(mat3(ubo.proj_view) * inNormals);
	mat3 normalize_worldspace = mat3(transpose(inverse(geometry_src.model)));


	vec3 normal = normalize(normalize_worldspace * inNormals);

	gl_Position = ubo.proj_view * pos_to_world;

	// FragPos = vec3(geometry_src.model * vec4(inPosition, 1.0));
	FragPos = pos_to_world.xyz;
	// gl_Position = (ubo.proj_view * geometry_src.model) * vec4(inPosition, 1.0);
	fragColor = vec4(inColor, 1.0);
	fragTexCoords = inTexCoords;
	fragNormals = normal;
	materialColor = geometry_src.color;
}