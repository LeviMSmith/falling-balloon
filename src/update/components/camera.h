#ifndef CAMERA_H_
#define CAMERA_H_

#include "core.h"

#include "utils/math/vec.h"

namespace Components {
  class Camera {
  public:
    Vec3<f32> pos;
    Vec3<f32> rotation;

    static Result create(Camera& camera);
    static void destroy(Camera& camera);
  };
}

#endif // CAMERA_H_
