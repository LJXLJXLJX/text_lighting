#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

out vec3 normal;
out vec2 tex_coord;
out vec3 frag_pos;

void main()
{
    gl_Position = vec4(a_pos, 1.0);
    frag_pos = a_pos;
    normal = a_normal;
    tex_coord = a_tex_coord;
}