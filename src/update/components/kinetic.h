#ifndef KINETIC_H_
#define KINETIC_H_

#include "core.h"

#include "utils/math/vec.h"

namespace Components {
  struct Kinetic {
    Vec3<f32> vel;
    Vec3<f32> acc;
  };
}

#endif // KINETIC_H_
