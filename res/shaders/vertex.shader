#version 410 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 tex_coor;

out VS_OUT
{
    vec2 tex_coor;
} vs_out;

uniform mat4 proj_mat;

void main()
{
    gl_Position = proj_mat * vec4(position, 0.0, 1.0);
    vs_out.tex_coor = tex_coor;
}