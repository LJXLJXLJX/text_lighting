#version 330 core
out vec4 frag_color;

in vec2 tex_coord;

uniform sampler2D screenTexture;

void main()
{
    vec3 col = texture(screenTexture, tex_coord).rgb;
    frag_color = vec4(col, 1.0);
} 