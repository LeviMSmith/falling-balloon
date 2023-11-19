#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertex_color;

void main() {
    gl_Position = vec4(triangle_verts[gl_VertexID], 0.0f, 1.0f);
    vertex_color = triangle_colors[gl_VertexID];
}

