#include "core.h"
#include "render/render.h"

#include "GLFW/glfw3.h"

Result create_window() {
  if (!glfwInit()) {
    return Result::FAILURE_GLFW_ERROR;
  }

  return Result::SUCCESS;
}
