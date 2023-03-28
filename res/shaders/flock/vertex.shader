#version 410 core

layout(location = 0) in vec2 position;
layout(location = 1) in float heading;

out VS_OUT
{
    vec2 position;
    float heading;
} vs_out;

void main()
{
    vs_out.position = position;
    vs_out.heading = heading;
}