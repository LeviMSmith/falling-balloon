#ifndef RENDER_H_
#define RENDER_H_

#include "core.h"

#include "utils/config/config.h"
#include "utils/data-structures/lru-cache.h"
#include "render/gl.h"
#include "render/mesh.h"
#include "event/event.h"
#include "ecs/ecs.h"

#include "GLFW/glfw3.h"

#include <vector>
#include <unordered_map>

class Render {
public:
  static Result create(Render*& render, const Config* const config);
  static void destroy(Render*& render);

  Result draw(WindowEvents* window_events, RenderEvents* render_events, ECS* ecs);
  Result present();

  void get_glfw_window(GLFWwindow*& glfw_window);
private:
  GLFWwindow* glfw_window;
  GlBackend* gl_backend;

  int frame_buffer_width, frame_buffer_height;

  DrawInfo draw_info;

  std::unordered_set<EntityID> chunks_in_view;
  std::unordered_set<EntityID> chunks_in_pipeline;

  std::vector<EntityID> get_loaded_chunks_in_view(const ECS* const ecs);
};

#endif // RENDER_H_
