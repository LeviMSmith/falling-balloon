#version 330 core

vec2 triangle_verts[3] = vec2[3](
  vec2(-0.5f, -0.5f),
  vec2(0.5f, -0.5f),
  vec2(0.0f, 0.5f)
);

vec3 triangle_colors[3] = vec3[3](
  vec3(1.0f, 0.0f, 0.0f),
  vec3(0.0f, 1.0f, 0.0f),
  vec3(0.0f, 0.0f, 1.0f)
);

out vec3 vertex_color;

void main() {
    gl_Position = vec4(triangle_verts[gl_VertexID], 0.0f, 1.0f);
    vertex_color = triangle_colors[gl_VertexID];
}

