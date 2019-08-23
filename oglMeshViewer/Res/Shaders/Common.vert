#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 uv;

out vec2 frag_uv;
out vec3 out_pos;
out vec3 out_normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    out_pos = vec3(model * vec4(pos, 1.0));
    out_normals = mat3(transpose(inverse(model))) * normals;

    frag_uv = uv;
    
	mat4 mvp = proj * view * model;
    gl_Position = mvp * vec4(pos, 1.0); 
}