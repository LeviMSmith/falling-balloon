#version 330 core

uniform sampler2D tex_atlas;

in vec2 uv;
out vec4 FragColor;

void main() {
    FragColor = texture(tex_atlas, uv);
}
