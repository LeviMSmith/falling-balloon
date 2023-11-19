#ifndef CHUNK_H_
#define CHUNK_H_

#include "core.h"

#include "render/mesh.h"
#include "ecs/entity.h"

#include <set>

enum Cell: u8 {
  NONE,
  DIRT,
};

namespace Components {
  constexpr size_t CHUNK_COMPONENT_CELL_WIDTH = 16;
  constexpr size_t CHUNK_COMPONENT_NUM_CELLS = CHUNK_COMPONENT_CELL_WIDTH * CHUNK_COMPONENT_CELL_WIDTH * CHUNK_COMPONENT_CELL_WIDTH;

  class Chunk {
  public:
    std::set<EntityID> entities;
    Cell cells[CHUNK_COMPONENT_NUM_CELLS];

    static Result create(Chunk& chunk);
    static void destroy(Chunk& chunk);

    Mesh generate_mesh(glm::vec3 model_pos);
  };
} // namespace Components

#endif // CHUNK_H_
