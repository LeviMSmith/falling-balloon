#include "core.h"
#include "render/gl.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

Result GlBackend::create(GlBackend*& gl_backend, GLFWwindow* glfw_window) {
  gl_backend = new GlBackend;

  // Can I just say that I hate stateful libraries?
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    GlBackend::destroy(gl_backend);
    LOG_ERROR("GlBackend failed to initialize GLEW:");
    LOG_ERROR((const char*)glewGetErrorString(err));
    return Result::FAILURE_GLEW_ERROR;
  }

  glfwMakeContextCurrent(glfw_window);

  return Result::SUCCESS;
}

void GlBackend::destroy(GlBackend*& gl_backend) {
  if (gl_backend != nullptr) {
    delete gl_backend;
    gl_backend = nullptr;
  }
}

Result GlBackend::draw() {
  glClear(GL_COLOR_BUFFER_BIT);

  return Result::SUCCESS;
}

Result GlBackend::present() {
  glfwSwapBuffers(glfw_window);
  return Result::SUCCESS;
}
