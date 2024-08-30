//we will be using glsl version 4.5 syntax
#vertex
#version 450

layout (location = 0) out vec3 outColor;

void main()
{
	//const array of positions for the triangle
	const vec3 positions[3] = vec3[3](
		vec3(1.f,1.f, 0.0f),
		vec3(-1.f,1.f, 0.0f),
		vec3(0.f,-1.f, 0.0f)
	);

	const vec3 colors[3] = vec3[](
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0),
		vec3(0.0, 0.0, 1.0)
	);

	//output the position of each vertex
	gl_Position = vec4(positions[gl_VertexIndex], 1.0f);
	outColor = colors[gl_VertexIndex];
}

#fragment
//glsl version 4.5
#version 450

//shader input
layout (location = 0) in vec3 inColor;

// output write]
layout(location = 0) out vec4 outFragColor;

void main()
{
	//return red
	// outFragColor = vec4(1.f,0.f,0.f,1.0f);
	outFragColor = vec4(inColor, 1.0);
}
