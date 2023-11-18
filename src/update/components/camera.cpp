#include "core.h"
#include "update/components/camera.h"

namespace Components {
  Result Camera::create(Camera& camera) {
    camera.pos.setX(0.0f);
    camera.pos.setY(0.0f);
    camera.pos.setZ(0.0f);

    camera.rotation.setX(0.0f);
    camera.rotation.setY(0.0f);
    camera.rotation.setZ(0.0f);

    return Result::SUCCESS;
  }

  void Camera::destroy(Camera& kinetic) {}
}
