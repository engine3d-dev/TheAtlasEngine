#type vertex
#version 330 core

layout (location = 0) in vec3 aPos;

void main(){
    gl_Position = vec4(aPos, 1.0);
}

#type fragment
#version 330 core

out vec4 FragColor;

// In OpenGL code we use uniforms to modify our colors through here
uniform vec4 v_Color;

void main(){
    // FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    FragColor = v_Color;
}
