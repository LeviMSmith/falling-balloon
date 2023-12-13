#include "core.h"
#include "ecs/components/chunk.h"

#include "render/mesh.h"
#include "utils/math/dim.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cstring>

void generate_face_vertices(std::vector<Mesh::Vertex>& vertices, u8 side, u8 x, u8 y, u8 z);

namespace Components {
  Chunk::Chunk() {
    std::memset(this->cells, Cell::NONE, CHUNK_COMPONENT_NUM_CELLS);
  }

  Chunk::~Chunk() {}

  Mesh Chunk::generate_mesh(glm::vec3 model_pos) {
    Mesh return_mesh;

    s16 neighbor_x = 0, neighbor_y = 0, neighbor_z = 0;
    for (u8 x = 0; x < CHUNK_COMPONENT_CELL_WIDTH; x++) {
      for (u8 y = 0; y < CHUNK_COMPONENT_CELL_WIDTH; y++) {
        for (u8 z = 0; z < CHUNK_COMPONENT_CELL_WIDTH; z++) {
          Cell cell = cells[Dim::threed_to_oned<size_t, u8>(x, y, z, CHUNK_COMPONENT_CELL_WIDTH, CHUNK_COMPONENT_CELL_WIDTH)];
          if (cell != Cell::NONE) {
            for (u8 side = 0; side < 6; side++) {
              switch (side) {
                case 0: { // bottom
                  neighbor_z--;
                  break;
                }
                case 1: { // front
                  neighbor_y--;
                  break;
                }
                case 2: { // left
                  neighbor_x--;
                  break;
                }
                case 3: { // top
                  neighbor_z++;
                  break;
                }
                case 4: { // back
                  neighbor_y++;
                  break;
                }
                case 5: { // right
                  neighbor_x++;
                  break;
                }
              }

              bool x_out_of_chunk = neighbor_x < 0 || neighbor_x >= CHUNK_COMPONENT_CELL_WIDTH;
              bool y_out_of_chunk = neighbor_y < 0 || neighbor_y >= CHUNK_COMPONENT_CELL_WIDTH;
              bool z_out_of_chunk = neighbor_z < 0 || neighbor_z >= CHUNK_COMPONENT_CELL_WIDTH;
              if (!z_out_of_chunk && !y_out_of_chunk && !x_out_of_chunk) {
                Cell neighbor_cell = cells[Dim::threed_to_oned<size_t, u8>(neighbor_x, neighbor_y, neighbor_z, CHUNK_COMPONENT_CELL_WIDTH, CHUNK_COMPONENT_CELL_WIDTH)];
                if (neighbor_cell == Cell::NONE) {
                  generate_face_vertices(return_mesh.vertices, side, x, y, z);
                }
              }
              else {
                generate_face_vertices(return_mesh.vertices, side, x, y, z);
              }
              // generate_face_vertices(return_mesh.vertices, side, x, y, z);
            }
          }
        }
      }
    }

    return_mesh.model = glm::mat4(1.0f);
    return_mesh.model = glm::translate(return_mesh.model, model_pos);

    // LOG_INFO("Num verticies: %d", return_mesh.vertices.size());
    
    return return_mesh;
  }
}


void generate_face_vertices(std::vector<Mesh::Vertex>& vertices, u8 side, u8 x, u8 y, u8 z) {
    // Define the offset for each vertex of the face
    static const glm::vec3 offsets[6][4] = {
        {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}}, // Bottom
        {{0, 0, 1}, {1, 0, 1}, {1, 0, 0}, {0, 0, 0}}, // Front
        {{0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 1, 0}}, // Left
        {{0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}}, // Top
        {{0, 0, 1}, {0, 1, 1}, {1, 1, 1}, {1, 0, 1}}, // Back
        {{1, 0, 0}, {1, 0, 1}, {1, 1, 1}, {1, 1, 0}}  // Right
    };

    static const glm::vec2 tex_coord = {1, 0};
    static const f32 atlas_size = 16.0f;
    static const f32 padding = 0.005f;
    static const glm::vec2 face_uv[4] = {
        {tex_coord.x / atlas_size + padding, tex_coord.y / atlas_size + padding},
        {(tex_coord.x + 1) / atlas_size - padding, tex_coord.y / atlas_size + padding},
        {(tex_coord.x + 1) / atlas_size - padding, (tex_coord.y + 1) / atlas_size - padding},
        {tex_coord.x / atlas_size + padding, (tex_coord.y + 1) / atlas_size - padding}
    };

    // Calculate the base position of the cell
    glm::vec3 base_position(x, y, z);

    // Add vertices for each face (two triangles)
    const glm::vec3* face_offsets = offsets[side];

    vertices.push_back(Mesh::Vertex{base_position + face_offsets[0], face_uv[0]});
    vertices.push_back(Mesh::Vertex{base_position + face_offsets[1], face_uv[1]});
    vertices.push_back(Mesh::Vertex{base_position + face_offsets[2], face_uv[2]});

    vertices.push_back(Mesh::Vertex{base_position + face_offsets[2], face_uv[2]});
    vertices.push_back(Mesh::Vertex{base_position + face_offsets[3], face_uv[3]});
    vertices.push_back(Mesh::Vertex{base_position + face_offsets[0], face_uv[0]});
}

void Components::Chunk::generate_cells(const ChunkGenInfo& gen_info) {
  const f32 OCEAN_LEVEL = -50.0f;
  const f32 BEACH_LEVEL = -45.0f;
  const f32 DIRT_LEVEL = -40.0f;

  glm::vec3 chunk_ws_pos = gen_info.pos * static_cast<s32>(CHUNK_COMPONENT_CELL_WIDTH);
  f32 chunk_ws_height = chunk_ws_pos.y;


  for (u8 y = 0; y < CHUNK_COMPONENT_CELL_WIDTH; y++) {
    f32 ws_height = y + chunk_ws_height;

    Cell value;
    
    if (ws_height < OCEAN_LEVEL) {
      value = Cell::WATER;
    } else if (ws_height < BEACH_LEVEL) {
      value = Cell::SAND;
    } else if (ws_height < DIRT_LEVEL) {
      value = Cell::DIRT;
    }

    for (u8 z = 0; z < CHUNK_COMPONENT_CELL_WIDTH; z++) {
      for (u8 x = 0; x < CHUNK_COMPONENT_CELL_WIDTH; x++) {
        u32 index = Dim::threed_to_oned<u32, u8>(x, y, z, CHUNK_COMPONENT_CELL_WIDTH, CHUNK_COMPONENT_CELL_WIDTH);
        cells[index] = value;
      }
    }
  }
}

