#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in uint tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vert_color;
out vec2 tex_uv;

vec2 get_tex_uv(uint texnorm) {
    const float atlas_size = 16.0f;

    uint cell = texnorm & uint(0xFF);
    uint side = (texnorm >> 8) & uint(0xFF);
    uint corner = (texnorm >> 16) & uint(0xFF);

    //return vec4(float(cell) / 255.0f, float(side) / 255.0f, float(corner) / 255.0f, float(test) / 255.0f);

    vec2 atlas_index = vec2(0, 0);

    //cell = uint(2);

    switch(cell) {
        case uint(0): // None        
            return vec2(0, 0);
        case uint(1): { // Grass
            switch(side) {
                case uint(0): // down
                    atlas_index = vec2(1, 0);
                    break;
                case uint(1): // front
                case uint(2): // left
                case uint(4): // back
                case uint(5): // right
                    atlas_index = vec2(2, 0);
                    break;
                case uint(3):
                    atlas_index = vec2(3, 0);
                    break;
            }
            break;
        }
        case uint(2): // Sand
            atlas_index = vec2(4, 0);
            break;
        case uint(3): // Water
            atlas_index = vec2(5, 0);
            break;
    }

    vec2 corner_offset = vec2(0, 0);
    const float padding = 0.05f;

    switch(corner) {
        case uint(0): // bottom left
            corner_offset = vec2(0 + padding, 0 + padding);
            break;
        case uint(1): // bottom right
            corner_offset = vec2(1.0f - padding, 0 + padding);
            break;
        case uint(2): // top right
            corner_offset = vec2(1.0f - padding, 1.0f - padding);
            break;
        case uint(3): // top left
            corner_offset = vec2(0 + padding, 1.0f - padding);
            break;
    }

    return (atlas_index + corner_offset) / atlas_size;
    //return vec2(1.0f/16.0f, 0.0f/16.0f) + (corner_offset / atlas_size);
}

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    tex_uv = get_tex_uv(tex);
}
