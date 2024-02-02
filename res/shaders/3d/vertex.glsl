#version 460 core
layout (location = 0) in vec3 pos;

out vec3 frag_pos;
out vec3 out_color;

uniform vec3 in_color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    frag_pos = vec3(model * vec4(pos, 1.0));
    out_color = in_color;
    gl_Position = projection * view * vec4(frag_pos, 1.0);
}