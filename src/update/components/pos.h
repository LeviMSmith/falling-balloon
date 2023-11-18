#ifndef POS_H_
#define POS_H_

#include "core.h"

#include "utils/math/vec.h"

namespace Components {
  class Pos {
  public:
    Vec3<f32> pos;

    static Result create(Pos& pos);
    static void destroy(Pos& pos);
  };
}

#endif // POS_H_
