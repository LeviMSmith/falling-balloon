#ifndef EVENT_H_
#define EVENT_H_

#include "core.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

struct Events {

};

class EventHandler {
public:
  static Result create(EventHandler*& event_handler, GLFWwindow* glfw_window);
  static void destroy(EventHandler*& event_handler);

  Result get_events(Events* events);
private:
  GLFWwindow* glfw_window;
};

#endif // EVENT_H_
