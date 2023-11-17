#ifndef CHUNK_H_
#define CHUNK_H_

#include "core.h"

#include "update/ecs/entity.h"

#include <set>

namespace Components {
  constexpr size_t CHUNK_COMPONENT_CELL_WIDTH = 16;

  struct Chunk {
    std::set<EntityID> entities;
    u8 cells[CHUNK_COMPONENT_CELL_WIDTH * CHUNK_COMPONENT_CELL_WIDTH * CHUNK_COMPONENT_CELL_WIDTH];
  };
}

#endif // CHUNK_H_
