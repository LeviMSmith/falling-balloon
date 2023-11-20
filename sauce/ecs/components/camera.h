#ifndef CAMERA_H_
#define CAMERA_H_

#include "core.h"

#include "glm/glm.hpp"

namespace Components {
struct Camera {
  Camera()
      : eye(glm::vec3(-3.0f, -3.0f, -3.0f)),
        up(glm::vec3(0.0f, 1.0f, 0.0f)), yaw(0),
        pitch(0) {
          update_view();
        }

  glm::vec3 eye, up;
  f32 yaw, pitch;
  glm::mat4 view;

  void update_view();
};
} // namespace Components

#endif // CAMERA_H_
