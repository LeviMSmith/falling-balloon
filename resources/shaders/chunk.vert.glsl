#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertex_color;
out vec2 tex_uv;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vertex_color = vec3(0.0, 1.0, 0.0);
    tex_uv = tex;
}
