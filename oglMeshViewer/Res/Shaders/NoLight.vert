#version 460 core
layout(location = 0) in vec4 pos;
layout(location = 2) in vec2 uv;

out vec2 out_uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main( )
{
    out_uv = uv;
    mat4 mvp = proj * view * model;
    gl_Position = mvp * pos;
}