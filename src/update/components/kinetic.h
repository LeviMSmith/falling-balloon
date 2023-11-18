#ifndef KINETIC_H_
#define KINETIC_H_

#include "core.h"

#include "utils/math/vec.h"

namespace Components {
  class Kinetic {
  public:
    Vec3<f32> vel;
    Vec3<f32> acc;

    static Result create(Kinetic& kinetic);
    static void destroy(Kinetic& kinetic);
  };
}

#endif // KINETIC_H_
