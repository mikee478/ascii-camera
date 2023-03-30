#version 410 core

in VS_OUT
{
    vec2 tex_coor;
} fs_in;

uniform sampler2D u_Texture;

out vec4 color;

void main()
{
    float v = 1.0 - texture(u_Texture, fs_in.tex_coor).r;
    color = vec4(v, v, v, 1.0);
}