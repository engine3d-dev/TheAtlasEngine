#type vertex
#version 330 core

// Utilized by glAttribPointer is where location mattters
layout (location = 0) in vec3 aPos; // position attribute at position 0
layout (location = 1) in vec3 aColor; // color  attribute position 1
layout (location = 2) in vec2 aTexCoord; // tex coord attribute at position 2


// specifying the color that will get outputted to our fragment shader
out vec3 vertexColor;
out vec2 TexCoord;

// Camera Data
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    // gl_Position = vec4(aPos, 1.0);
    gl_Position = projection * view * vec4(aPos, 1.0);
    vertexColor = aColor;
    // TexCoord = aTexCoord;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}

#type fragment
#version 330 core
out vec4 FragColor;

// Our input vertex color received from our vertex shader (must be same name and same type)
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D inputTexture; // Texture that we give this fragment shader to render from OpenGL.
uniform vec4 inputColor;

void main(){
    FragColor = texture(inputTexture, TexCoord) * inputColor;
}