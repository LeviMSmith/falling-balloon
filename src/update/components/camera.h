#ifndef CAMERA_H_
#define CAMERA_H_

#include "core.h"

#include "utils/math/vec.h"

namespace Components {
  struct Camera {
    Vec3<f32> pos;
    Vec3<f32> rotation;
  };
}

#endif // CAMERA_H_
