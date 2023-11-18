#include "core.h"
#include "render/gl.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

Result GlBackend::create(GlBackend*& gl_backend, GLFWwindow* glfw_window) {
  gl_backend = new GlBackend;
  gl_backend->glfw_window = glfw_window;

  glfwMakeContextCurrent(glfw_window);

  // Can I just say that I hate stateful libraries
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    GlBackend::destroy(gl_backend);
    LOG_ERROR("GlBackend failed to initialize GLEW:");
    LOG_ERROR((const char*)glewGetErrorString(err));
    return Result::FAILURE_GLEW_ERROR;
  }

  gl_backend->handle_resize();

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

void GlBackend::handle_resize(int width, int height) {
  LOG_INFO("GL backend handling framebuffer resize");

  // It is possible that GLFW hands us these values from the callback,
  // but just to make sure it's not the default we'll grab the values
  // from glfw again.
  if (width == 0 || height == 0) {
    LOG_DEBUG("Grabbing framebuffer size from GLFW");
    glfwGetFramebufferSize(glfw_window, &width, &height);
  }

  glViewport(0, 0, width, height);
}
