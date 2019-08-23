#version 460 core
out vec4 color;
in vec2 out_uv;

uniform sampler2D texture_diffuse1;

void main()
{
    color = texture(texture_diffuse1, out_uv);
}