#version 330 core

uniform sampler2D tex_atlas;

in vec2 tex_uv;
out vec4 FragColor;

void main() {
    FragColor = texture(tex_atlas, tex_uv);
}
