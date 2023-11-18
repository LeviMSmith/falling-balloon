#ifndef RENDER_H_
#define RENDER_H_

#include "core.h"
#include "utils/config/config.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

class Render {
public:
  static Result create(Render*& render, const Config* const config);
  static void destroy(Render* render);

private:
  GLFWwindow* glfw_window;
};

#endif // RENDER_H_
