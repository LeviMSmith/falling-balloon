#include "core.h"
#include "update/components/camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Components {
  void Camera::update_view() {
    view = glm::lookAt(eye, center, up);
  }
}
