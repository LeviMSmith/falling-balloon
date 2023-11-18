#include "core.h"
#include "event/event.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

Result EventHandler::create(EventHandler*& event_handler, GLFWwindow* glfw_window) {
  event_handler = new EventHandler;

  event_handler->glfw_window = glfw_window;

  return Result::SUCCESS;
}

void EventHandler::destroy(EventHandler*& event_handler) {
  if (event_handler != nullptr) {
    delete event_handler;
    event_handler = nullptr;
  }
}

Result EventHandler::get_events(Events* events) {
  return Result::SUCCESS;
}
