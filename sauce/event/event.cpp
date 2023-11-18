#include "core.h"
#include "event/event.h"

#include "GLFW/glfw3.h"

Events events;

void error_callback(int error_code, const char* description);
void window_close_callback(GLFWwindow* window);
void window_resize_callback(GLFWwindow* window, int width, int height);
void window_maximized_callback(GLFWwindow* window, int maximized);

Result EventHandler::create(EventHandler*& event_handler, GLFWwindow* glfw_window) {
  event_handler = new EventHandler;

  event_handler->glfw_window = glfw_window;

  glfwSetErrorCallback((GLFWerrorfun)error_callback);
  glfwSetWindowCloseCallback(glfw_window, (GLFWwindowclosefun)window_close_callback);
  glfwSetFramebufferSizeCallback(glfw_window, (GLFWwindowsizefun)window_resize_callback);
  glfwSetWindowMaximizeCallback(glfw_window, window_maximized_callback);

  return Result::SUCCESS;
}

void EventHandler::destroy(EventHandler*& event_handler) {
  if (event_handler != nullptr) {
    delete event_handler;
    event_handler = nullptr;
  }
}

void EventHandler::get_events(Events* out_events) {
  clear_events();
  glfwPollEvents();

  *out_events = events;
}

void EventHandler::clear_events() {
  events.window_events.clear();
}

void error_callback(int error_code, const char* description) {
  LOG_ERROR(description);
}

void window_close_callback(GLFWwindow* window) {
  events.window_events.push_back(Event::WINDOW_SHOULD_CLOSE);
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
  events.window_events.push_back(Event::WINDOW_RESIZED);
  events.window_size.width = width;
  events.window_size.height = height;
}

void window_maximized_callback(GLFWwindow* window, int maximized) {
  if (maximized) {
    events.window_events.push_back(Event::WINDOW_MAXIMIZED);
  }
  else {
    events.window_events.push_back(Event::WINDOW_UNMAXIMIZED);
  }
}
