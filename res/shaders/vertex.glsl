#version 460 core
layout (location = 0) in vec3 pos;
out vec3 out_color;

uniform mat4 model;
uniform vec3 in_color;


void main()
{
    gl_Position = model * vec4(pos, 1.0);
    out_color = in_color;
}