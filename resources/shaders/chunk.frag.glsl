#version 330 core

uniform sampler2D tex_atlas;

in vec2 tex_uv;
in vec4 vert_color;
out vec4 FragColor;

void main() {
    FragColor = texture(tex_atlas, tex_uv);
    //FragColor = vert_color;
}
