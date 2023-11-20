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

Result Render::draw(WindowEvents* window_events, RenderEvents* render_events, ECS* ecs) {
  for (WindowEvent event : window_events->events) {
    switch (event) {
      case WindowEvent::SHOULD_CLOSE: {
        return Result::RENDER_WINDOW_SHOULD_CLOSE;
      }
      case WindowEvent::RESIZED:
        frame_buffer_width = window_events->window_size.width;
        frame_buffer_height = window_events->window_size.height;
        gl_backend->handle_resize(frame_buffer_width, frame_buffer_height);
        break;
      case WindowEvent::MAXIMIZED:
      case WindowEvent::UNMAXIMIZED:
        break;
      default:
        break;
    }
  }

  draw_info.clear();

  bool update_chunk_pipeline = false;
  for (RenderEvent event : render_events->events) {
    switch (event) {
      case RenderEvent::CAMERA_UPDATED: {
        std::vector<EntityID> list_chunks_in_view = get_loaded_chunks_in_view(ecs);
        chunks_in_view = std::unordered_set<EntityID>(list_chunks_in_view.begin(), list_chunks_in_view.end());
        update_chunk_pipeline = true;
        draw_info.new_view = render_events->view;
        break;
      }
      case RenderEvent::CHUNK_MESH_INVALIDATED: {
        // Chunks in view could be updated after this, but this works for now.
        for (EntityID entity_id : render_events->invalidated_chunk_meshes) {
          if (chunks_in_view.contains(entity_id)) {
            update_chunk_pipeline = true;
          }
        }
        break;
      }
    }
  }

  if (update_chunk_pipeline) {
    std::vector<EntityID> chunks_in_view_list(chunks_in_view.begin(), chunks_in_view.end());
    draw_info.new_chunk_meshes = ecs->get_chunk_mesh_component_batch(chunks_in_view_list);
    
    chunks_in_pipeline = chunks_in_view;
  }
  
  return gl_backend->draw(draw_info);
}

Result Render::present() {
  return gl_backend->present();
}

void Render::get_glfw_window(GLFWwindow*& glfw_window) {
  glfw_window = this->glfw_window;
}

std::vector<EntityID> Render::get_loaded_chunks_in_view(const ECS* const ecs) {
  // Right now we just want to check the rendering so do them all
  // TODO: actually implement
  std::vector<EntityID> return_entities;
  return_entities.reserve(ecs->chunk_components.size());
  for (const auto& [entity_id, chunk] : ecs->chunk_components) {
    return_entities.push_back(entity_id);
  }

  return return_entities;
}
