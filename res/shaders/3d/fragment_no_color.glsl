#version 460 core
out vec4 FragColor;

in vec3 frag_pos;
in vec2 frag_tex_coords;

uniform sampler2D pyramyd_texture;

void main()
{
    FragColor = vec4(texture(pyramyd_texture, frag_tex_coords).rgb, 1.0f);
}