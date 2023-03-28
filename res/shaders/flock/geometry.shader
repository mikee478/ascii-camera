#version 410 core

layout (points) in;
layout (line_strip, max_vertices = 32) out;

#define TWO_PI 6.28318530718
#define VERT_THETA 2.1

uniform mat4 u_proj_mat;

in VS_OUT
{
    vec2 position;
    float heading;
} gs_in[];

vec2 rotate(vec2 v, float a) {
	float s = sin(a);
	float c = cos(a);
	mat2 m = mat2(c, s, -s, c);
	return m * v;
}

void main()
{
    vec2 forward = rotate(vec2(1.0, 0.0), gs_in[0].heading);
    vec2 back_left = rotate(vec2(1.0, 0.0), gs_in[0].heading + VERT_THETA);
    vec2 back_right = rotate(vec2(1.0, 0.0), gs_in[0].heading - VERT_THETA);

    vec2 p_center = gs_in[0].position;

    vec2 p_forward = p_center + forward * 7.0;
    vec2 p_back_left = p_center + back_left * 5.0;
    vec2 p_back_right = p_center + back_right * 5.0;

    gl_Position = u_proj_mat * vec4(p_forward, 0.0, 1.0);
    EmitVertex();

    gl_Position = u_proj_mat * vec4(p_back_left, 0.0, 1.0);
    EmitVertex();

    gl_Position = u_proj_mat * vec4(p_center, 0.0, 1.0);
    EmitVertex();

    gl_Position = u_proj_mat * vec4(p_back_right, 0.0, 1.0);
    EmitVertex();

    gl_Position = u_proj_mat * vec4(p_forward, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();

    return;

    const int N = 20;
    const float DIST = 100.0;
    const float THETA = TWO_PI / N;
    vec2 p = vec2(DIST, 0.0);
    for(int i = 0;i<=N;i++)
    {
        p = rotate(p, THETA);
        gl_Position = u_proj_mat * vec4(p_center + p, 0.0, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}