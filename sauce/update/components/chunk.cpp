#include "core.h"
#include "update/components/chunk.h"

namespace Components {
  Result Chunk::create(Chunk& chunk) {
    return Result::SUCCESS;
  }

  void Chunk::destroy(Chunk& chunk) {}
}
