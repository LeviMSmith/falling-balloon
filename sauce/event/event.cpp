#include "core.h"
#include "event/event.h"

#include "GLFW/glfw3.h"

WindowEvents events;

void error_callback(int error_code, const char* description);
void window_close_callback(GLFWwindow* window);
void window_resize_callback(GLFWwindow* window, int width, int height);
void window_maximized_callback(GLFWwindow* window, int maximized);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void RenderEvents::clear() {
  events.clear();
  invalidated_chunk_meshes.clear();
}

Result EventHandler::create(EventHandler*& event_handler, GLFWwindow* glfw_window) {
  event_handler = new EventHandler;

  event_handler->glfw_window = glfw_window;

  glfwSetErrorCallback((GLFWerrorfun)error_callback);
  glfwSetWindowCloseCallback(glfw_window, (GLFWwindowclosefun)window_close_callback);
  glfwSetFramebufferSizeCallback(glfw_window, (GLFWwindowsizefun)window_resize_callback);
  glfwSetWindowMaximizeCallback(glfw_window, window_maximized_callback);

  glfwSetKeyCallback(glfw_window, key_callback);

  return Result::SUCCESS;
}

void EventHandler::destroy(EventHandler*& event_handler) {
  if (event_handler != nullptr) {
    delete event_handler;
    event_handler = nullptr;
  }
}

void EventHandler::get_events(WindowEvents* out_events) {
  clear_events();
  glfwPollEvents();

  *out_events = events;
}

void EventHandler::clear_events() {
  events.events.clear();
  events.key_presses.clear();
}

void error_callback(int error_code, const char* description) {
  LOG_ERROR(description);
}

void window_close_callback(GLFWwindow* window) {
  events.events.push_back(WindowEvent::SHOULD_CLOSE);
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
  events.events.push_back(WindowEvent::RESIZED);
  events.window_size.width = width;
  events.window_size.height = height;
}

void window_maximized_callback(GLFWwindow* window, int maximized) {
  if (maximized) {
    events.events.push_back(WindowEvent::MAXIMIZED);
  }
  else {
    events.events.push_back(WindowEvent::UNMAXIMIZED);
  }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  events.key_presses.push_back(WindowEvents::KeyPress{key, scancode, action, mods});
}
