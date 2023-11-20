#include "core.h"
#include "ecs/components/kinetic.h"

namespace Components {
  Result Kinetic::create(Kinetic& kinetic) {
    kinetic.vel.setX(0.0f);
    kinetic.vel.setY(0.0f);
    kinetic.vel.setZ(0.0f);

    kinetic.acc.setX(0.0f);
    kinetic.acc.setY(0.0f);
    kinetic.acc.setZ(0.0f);

    return Result::SUCCESS;
  }

  void Kinetic::destroy(Kinetic& kinetic) {}
}
