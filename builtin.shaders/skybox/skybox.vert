#version 460

#extension GL_EXT_buffer_reference : require
#extension GL_EXT_scalar_block_layout : require

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormals;
layout(location = 3) in vec2 inTexCoords;

layout (location=0) out vec3 fragDirectionCoords;

layout(buffer_reference, scalar) buffer readonly SceneEnvironmentUniforms {
	mat4 view_proj;
};

layout(push_constant) uniform Constants {
    SceneEnvironmentUniforms global_environment;
} push_const;

void main() {
	fragDirectionCoords = inPosition;
	SceneEnvironmentUniforms ubo = push_const.global_environment;
	vec4 pos = ubo.view_proj * vec4(inPosition, 1.0);
	gl_Position = pos.xyww;
}