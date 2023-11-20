#ifndef CAMERA_H_
#define CAMERA_H_

#include "core.h"

#include "glm/glm.hpp"

namespace Components {
struct Camera {
  Camera()
      : eye(glm::vec3(-1.0f, -1.0f, -1.0f)),
        direction(glm::vec3(0.0f, 0.0f, 1.0f)), 
        up(glm::vec3(0.0f, 1.0f, 0.0f)) {
          update_view();
        }

  glm::vec3 eye, direction, up;
  glm::mat4 view;

  void update_view();
};
} // namespace Components

#endif // CAMERA_H_
