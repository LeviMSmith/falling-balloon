#include "core.h"
#include "render/render.h"

#include "utils/config/config.h"
#include "utils/threadpool.h"
#include "ecs/components/chunk.h"
#include "render/gl.h"

#include "GLFW/glfw3.h"

#include <functional>
#include <future>
#include <vector>

Result Render::create(Render*& render, const Config* const config) {
  render = new Render;
  render->glfw_window = nullptr;
  render->gl_backend = nullptr;

  if (!glfwInit()) {
    Render::destroy(render);
    LOG_ERROR("GLFW failed to initialize");
    return Result::FAILURE_GLFW_ERROR;
  }

  // if (!glfwVulkanSupported()) {
  //   Render::destroy(render);
  //   LOG_ERROR("GLFW is not supporting Vulkan. Is there a loader available?");
  //   return Result::FAILURE_GLFW_ERROR;
  // }
  
#ifndef NDEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  if (config->window_start_maximixed) {
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  }
  render->glfw_window = glfwCreateWindow(config->window_width, config->window_height, "Falling Balloon", NULL, NULL);

  Result gl_backend_create_res = GlBackend::create(render->gl_backend, render->glfw_window);
  if (gl_backend_create_res != Result::SUCCESS) {
    Render::destroy(render);
    LOG_ERROR("Render failed to create GL backend");
    return gl_backend_create_res;
  }

  return Result::SUCCESS;
}

void Render::destroy(Render*& render) {
  if (render != nullptr) {
    GlBackend::destroy(render->gl_backend);

    if (render->glfw_window != nullptr) {
      glfwDestroyWindow(render->glfw_window);
    }
    glfwTerminate();

    delete render;
    render = nullptr;
  }
}

Result Render::draw(Events* events, const ECS* const ecs) {
  for (Event event : events->window_events) {
    switch (event) {
      case Event::WINDOW_SHOULD_CLOSE: {
        return Result::RENDER_WINDOW_SHOULD_CLOSE;
      }
      case Event::WINDOW_RESIZED:
        frame_buffer_width = events->window_size.width;
        frame_buffer_height = events->window_size.height;
        gl_backend->handle_resize(frame_buffer_width, frame_buffer_height);
        break;
      case Event::WINDOW_MAXIMIZED:
      case Event::WINDOW_UNMAXIMIZED:
        break;
    }
  }

  DrawInfo draw_info;
  std::vector<EntityID> chunks_to_render = get_chunks_to_render(ecs);
  draw_info.updated_chunk_meshes = chunk_handler.get_chunk_meshes(chunks_to_render, ecs);

  return gl_backend->draw(draw_info);
}

Result Render::present() {
  return gl_backend->present();
}

void Render::get_glfw_window(GLFWwindow*& glfw_window) {
  glfw_window = this->glfw_window;
}

std::unordered_map<EntityID, Mesh> Render::ChunkHandler::get_chunk_meshes(const std::vector<EntityID>& entity_ids, const ECS* const ecs) {
  std::unordered_map<EntityID, Mesh> return_meshes;

  Mesh mesh;
  std::unordered_map<EntityID, std::future<Mesh>> mesh_futures;

  for (EntityID entity_id : entity_ids) {
    if (mesh_cache.get(entity_id, mesh)) {
      return_meshes[entity_id] = mesh;
    }
    else {
      Components::Chunk chunk = ecs->chunk_components.at(entity_id);
      auto task = std::bind(&Components::Chunk::generate_mesh, &chunk);
      mesh_futures[entity_id] = ThreadPool::enqueue(task);
    }
  }

  for (auto& mesh_future : mesh_futures) {
    mesh = mesh_future.second.get();
    return_meshes[mesh_future.first] = mesh;
    mesh_cache.put(mesh_future.first, mesh);
  }

  return return_meshes;
}

std::vector<EntityID> Render::get_chunks_to_render(const ECS* const ecs) {
  std::vector<EntityID> return_entities;

  for (const auto& [entity_id, chunk] : ecs->chunk_components)  {
    return_entities.push_back(entity_id);
  }

  return return_entities;
}
