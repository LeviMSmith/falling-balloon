#include "core.h"
#include "render/render.h"

#include "utils/config/config.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

Result Render::create(Render*& render, const Config* const config) {
  render = new Render;
  render->glfw_window = nullptr;

  if (!glfwInit()) {
    Render::destroy(render);
    render = nullptr;
    LOG_ERROR("GLFW failed to initialize");
    return Result::FAILURE_GLFW_ERROR;
  }

  if (!glfwVulkanSupported()) {
    Render::destroy(render);
    render = nullptr;
    LOG_ERROR("GLFW is not supporting Vulkan. Is there a loader available?");
    return Result::FAILURE_GLFW_ERROR;
  }

  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  if (config->window_start_maximixed) {
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  }
  render->glfw_window = glfwCreateWindow(config->window_width, config->window_height, "Falling Balloon", NULL, NULL);

  return Result::SUCCESS;
}

void Render::destroy(Render* render) {
  if (render != nullptr) {
    if (render->glfw_window != nullptr) {
      glfwDestroyWindow(render->glfw_window);
    }
    glfwTerminate();

    delete render;
  }
}

void Render::get_glfw_window(GLFWwindow*& glfw_window) {
  glfw_window = this->glfw_window;
}
