#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormals;
layout(location = 3) in vec2 inTexCoords;

layout (location=0) out vec3 TexCoords;

layout (set = 0, binding = 0) readonly uniform UniformBuffer {
	mat4 view_proj;
} ubo;

// const vec3 pos[8] = vec3[8](
// 	vec3(-1.0,-1.0, 1.0),		// 0
// 	vec3( 1.0,-1.0, 1.0),		// 1
// 	vec3( 1.0, 1.0, 1.0),		// 2
// 	vec3(-1.0, 1.0, 1.0),		// 3

// 	vec3(-1.0,-1.0,-1.0),		// 4
// 	vec3( 1.0,-1.0,-1.0),		// 5
// 	vec3( 1.0, 1.0,-1.0),		// 6
// 	vec3(-1.0, 1.0,-1.0)		// 7
// );

// const int indices[36] = int[36](
// 	1, 0, 2, 3, 2, 0,	// front
// 	5, 1, 6, 2, 6, 1,	// right 
// 	6, 7, 5, 4, 5, 7,	// back
// 	0, 4, 3, 7, 3, 4,	// left
// 	5, 4, 1, 0, 1, 4,	// bottom
// 	2, 3, 6, 7, 6, 3	// top
// );

void main() {
	// int idx = indices[gl_VertexIndex];
	// vec4 Pos = vec4(pos[idx], 1.0);
	// vec4 WVP_Pos = ubo.view_proj * Pos;
	// gl_Position = WVP_Pos.xyww;


	// TexCoords = pos[idx].xyz;
	// TODO: For some reason the direction is off
	// Reference to OpenGL tutorial for correcting this skybox implementation!!
	// Dir = pos[idx].xyz;

	TexCoords = inPosition;
	vec4 pos = ubo.view_proj * vec4(inPosition, 1.0);
	gl_Position = pos.xyww;
}