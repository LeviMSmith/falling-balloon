#ifndef GL_H_
#define GL_H_

#include "core.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class GlBackend {
public:
  static Result create(GlBackend*& gl_backend, GLFWwindow* glfw_window);
  static void destroy(GlBackend*& gl_backend);

  Result draw();
  Result present();

  void handle_resize(int width = 0, int height = 0);
private:
  GLFWwindow* glfw_window;
};

#endif
