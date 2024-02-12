#version 460 core
out vec4 FragColor;

in vec3 frag_pos;
in vec2 frag_tex_coords;
in vec3 out_color;

uniform sampler2D texture;

void main()
{
    FragColor = vec4(out_color * texture(texture, frag_tex_coords).rgb, 1.0f);
}