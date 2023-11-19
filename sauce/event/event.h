#ifndef EVENT_H_
#define EVENT_H_

#include "core.h"

#include "ecs/entity.h"

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>

// Render events are specifically from Update module to
// Render module.
enum RenderEvent {
  CHUNK_MESH_INVALIDATED,
  CAMERA_UPDATED,
};

struct RenderEvents {
  std::vector<RenderEvent> events;
  
  std::vector<EntityID> invalidated_chunk_meshes;
  glm::mat4 view;
};

// Window events come from the event handler and generally
// go to the Render module.
enum WindowEvent {
  SHOULD_CLOSE,
  RESIZED,
  MAXIMIZED,
  UNMAXIMIZED,
};

struct WindowSize {
  int width;
  int height;
};

struct WindowEvents {
  std::vector<WindowEvent> events;

  WindowSize window_size;
};

class EventHandler {
public:
  static Result create(EventHandler*& event_handler, GLFWwindow* glfw_window);
  static void destroy(EventHandler*& event_handler);

  static void get_events(WindowEvents* events);
private:
  GLFWwindow* glfw_window;

  static void clear_events();
};

#endif // EVENT_H_
