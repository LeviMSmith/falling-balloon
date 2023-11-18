#ifndef EVENT_H_
#define EVENT_H_

#include "core.h"

#include <vector>

#include "GLFW/glfw3.h"

enum Event {
  WINDOW_SHOULD_CLOSE,
  WINDOW_RESIZED,
  WINDOW_MAXIMIZED,
  WINDOW_UNMAXIMIZED,
};

struct WindowSize {
  int width;
  int height;
};

struct Events {
  std::vector<Event> window_events;

  WindowSize window_size;
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
