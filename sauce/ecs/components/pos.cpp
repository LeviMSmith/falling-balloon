#include "core.h"
#include "ecs/components/pos.h"

namespace Components {
  Result Pos::create(Pos& pos) {
    pos.pos.setX(0.0f);
    pos.pos.setY(0.0f);
    pos.pos.setZ(0.0f);

    return Result::SUCCESS;
  }

  void Pos::destroy(Pos& pos) {}
}
