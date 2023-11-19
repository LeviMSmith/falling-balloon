#include "core.h"
#include "update/components/chunk.h"

#include "render/mesh.h"
#include "utils/math/dim.h"

void generate_face_verticies(std::vector<Mesh::Vertex>& verticies, u8 side, u8 x, u8 y, u8 z);

namespace Components {
  Result Chunk::create(Chunk& chunk) {
    return Result::SUCCESS;
  }

  void Chunk::destroy(Chunk& chunk) {}

  Mesh Chunk::generate_mesh() {
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
                  generate_face_verticies(return_mesh.verticies, side, x, y, z);
                }
              }
              else {
                generate_face_verticies(return_mesh.verticies, side, x, y, z);
              }
            }
          }
        }
      }
    }
    
    return return_mesh;
  }
}


void generate_face_vertices(std::vector<Mesh::Vertex>& vertices, int side, u8 x, u8 y, u8 z) {
    // Define the offset for each vertex of the face
    static const glm::vec3 offsets[6][4] = {
        {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}}, // Bottom
        {{0, 0, 1}, {1, 0, 1}, {1, 0, 0}, {0, 0, 0}}, // Front
        {{0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 1, 0}}, // Left
        {{0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}}, // Top
        {{0, 0, 1}, {0, 1, 1}, {1, 1, 1}, {1, 0, 1}}, // Back
        {{1, 0, 0}, {1, 0, 1}, {1, 1, 1}, {1, 1, 0}}  // Right
    };

    // Calculate the base position of the cell
    glm::vec3 basePosition(x, y, z);

    // Add vertices for each face (two triangles)
    const glm::vec3* faceOffsets = offsets[side];
    vertices.push_back(Mesh::Vertex{basePosition + faceOffsets[0]});
    vertices.push_back(Mesh::Vertex{basePosition + faceOffsets[1]});
    vertices.push_back(Mesh::Vertex{basePosition + faceOffsets[2]});

    vertices.push_back(Mesh::Vertex{basePosition + faceOffsets[2]});
    vertices.push_back(Mesh::Vertex{basePosition + faceOffsets[3]});
    vertices.push_back(Mesh::Vertex{basePosition + faceOffsets[0]});
}

