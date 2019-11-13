#version 330 core
out vec4 frag_color;

in vec2 tex_coord;

uniform sampler2D screen_texture;

void main()
{
    vec3 col = texture(screen_texture, tex_coord).rgb;
    frag_color = vec4(col, 1.0);
	//frag_color=vec4(1,0,0,0);
} 