#ifndef RENDER_H_
#define RENDER_H_

#include "core.h"

#include "utils/config/config.h"
#include "utils/data-structures/lru-cache.h"
#include "render/gl.h"
#include "render/mesh.h"
#include "event/event.h"
#include "update/ecs/ecs.h"

#include "GLFW/glfw3.h"

#include <vector>
#include <unordered_map>

class Render {
public:
  static Result create(Render*& render, const Config* const config);
  static void destroy(Render*& render);

  Result draw(Events* events, const ECS* const ecs);
  Result present();

  void get_glfw_window(GLFWwindow*& glfw_window);
private:
  class ChunkHandler {
  public:
    ChunkHandler() : mesh_cache(LRUcache<EntityID, Mesh>(MAX_CHUNK_ENTITIES)) {}

    std::unordered_map<EntityID, Mesh> get_chunk_meshes(const std::vector<EntityID>& entity_ids, const ECS* const ecs);

  private:
    LRUcache<EntityID, Mesh> mesh_cache;
  };

  GLFWwindow* glfw_window;
  GlBackend* gl_backend;

  ChunkHandler chunk_handler;

  int frame_buffer_width, frame_buffer_height;

  std::vector<EntityID> get_chunks_to_render(const ECS* const ecs);
};

#endif // RENDER_H_
