#include "core.h"
#include "ecs/components/camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Components {
  void Camera::update_view() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(direction);

    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(forward);

    right = glm::normalize(glm::cross(forward, up));

    view = glm::lookAt(eye, eye + direction, up);
  }
}
