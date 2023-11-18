#ifndef RENDER_H_
#define RENDER_H_

#include "core.h"
#include "utils/config/config.h"

#include "render/gl.h"

#include "GLFW/glfw3.h"

class Render {
public:
  static Result create(Render*& render, const Config* const config);
  static void destroy(Render*& render);

  Result draw();
  Result present();

  void get_glfw_window(GLFWwindow*& glfw_window);
private:
  GLFWwindow* glfw_window;

  GlBackend* gl_backend;
};

#endif // RENDER_H_
