#ifndef RENDER_H_
#define RENDER_H_

#include "core.h"

#include "utils/config/config.h"
#include "utils/data-structures/lru-cache.h"
#include "render/gl.h"
#include "event/event.h"
#include "update/ecs/ecs.h"

#include "GLFW/glfw3.h"

class Render {
public:
  static Result create(Render*& render, const Config* const config);
  static void destroy(Render*& render);

  Result draw(Events* events, ECS* ecs);
  Result present();

  void get_glfw_window(GLFWwindow*& glfw_window);
private:
  class ChunkHandler {
  public:
    ChunkHandler() : mesh_cache(LRUcache<EntityID, Mesh>(MAX_CHUNK_ENTITIES)) {}

    Result get_chunk_meshes(std::vector<EntityID>& entity_ids, ECS* ecs);

  private:
    LRUcache<EntityID, Mesh> mesh_cache;
  };

  GLFWwindow* glfw_window;
  GlBackend* gl_backend;

  ChunkHandler chunk_handler;

  int frame_buffer_width, frame_buffer_height;
};

#endif // RENDER_H_
