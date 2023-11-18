#ifndef RENDER_H_
#define RENDER_H_

#include "core.h"
#include "utils/config/config.h"

#include "render/gl.h"
#include "event/event.h"

#include "GLFW/glfw3.h"

class Render {
public:
  static Result create(Render*& render, const Config* const config);
  static void destroy(Render*& render);

  Result draw(Events* events);
  Result present();

  void get_glfw_window(GLFWwindow*& glfw_window);
private:
  GLFWwindow* glfw_window;

  GlBackend* gl_backend;

  int frame_buffer_width, frame_buffer_height;
};

#endif // RENDER_H_
