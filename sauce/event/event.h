#ifndef EVENT_H_
#define EVENT_H_

#include "core.h"

#include <vector>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

enum Event {
  GLFW_WINDOW_SHOULD_CLOSE  
};

struct Events {
  std::vector<Event> window_events;
};

class EventHandler {
public:
  static Result create(EventHandler*& event_handler, GLFWwindow* glfw_window);
  static void destroy(EventHandler*& event_handler);

  static void get_events(Events* events);
  static void clear_events();
private:
  GLFWwindow* glfw_window;
};

#endif // EVENT_H_
