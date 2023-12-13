#ifndef CHUNK_H_
#define CHUNK_H_

#include "core.h"

#include "render/mesh.h"
#include "ecs/entity.h"

#include <set>

enum Cell: u8 {
  NONE,
  DIRT,
  SAND,
  WATER,
};

struct ChunkGenInfo {
  glm::ivec3 pos;
};

namespace Components {
  constexpr u8 CHUNK_COMPONENT_CELL_WIDTH = 16;
  constexpr u32 CHUNK_COMPONENT_NUM_CELLS = CHUNK_COMPONENT_CELL_WIDTH * CHUNK_COMPONENT_CELL_WIDTH * CHUNK_COMPONENT_CELL_WIDTH;

  class Chunk {
  public:
    std::set<EntityID> entities;
    Cell cells[CHUNK_COMPONENT_NUM_CELLS];

    Chunk ();
    ~Chunk ();

    Mesh generate_mesh(glm::vec3 model_pos);
    void generate_cells(const ChunkGenInfo& gen_info);
  };
} // namespace Components

#endif // CHUNK_H_
