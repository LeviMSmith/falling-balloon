#include "core.h"
#include "ecs/components/camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Components {
  void Camera::update_view() {
    direction = glm::normalize(direction);
    view = glm::lookAt(eye, eye + direction, up);
  }
}
