#include "core.h"
#include "update/update.h"

#include "ecs/ecs.h"

#include "glm/glm.hpp"

Result Update::create(Update*& update, ECS* ecs) {
  update = new Update;
  Result player_create_res = ecs->create_entity_player(update->active_player);
  if (player_create_res != Result::SUCCESS) {
    return player_create_res;
  }

  return Result::SUCCESS;
}

void Update::destroy(Update* update) {
  if (update != nullptr) {
    delete update;
  }
}

Result Update::update(WindowEvents* window_events, RenderEvents* render_events, ECS* ecs) {
  static double old_mouse_x = 0.5, old_mouse_y = 0.5;

  render_events->clear();

  // Start by handling window events
  for (WindowEvent event : window_events->events) {
    switch (event) {
      case WindowEvent::MOUSE_MOVEMENT: {
        handle_mouse_movement(window_events->mouse_x - old_mouse_x, window_events->mouse_y - old_mouse_y, render_events, ecs);
        old_mouse_x = window_events->mouse_x;
        old_mouse_y = window_events->mouse_y;
      }
      default:
        break;
    }
  }

  if (window_events->key_presses.size() > 0) {
    Result keypress_res = handle_keypresses(window_events->key_presses, render_events, ecs);
    if (keypress_res != Result::SUCCESS) {
      return keypress_res;
    }
  }

  load_chunks(ecs);

  return Result::SUCCESS;
}


Result Update::handle_keypresses(std::vector<WindowEvents::KeyPress>& key_presses, RenderEvents* render_events, ECS* ecs) {
  static f32 move_speed = 1.0f; // Adjust as needed for movement speed
  b8 camera_updated = false;
  Components::Camera& active_camera = ecs->camera_components[active_player];

  glm::vec3 forward = active_camera.forward;
  glm::vec3 right = active_camera.right;
  glm::vec3 up = {0.0f, 1.0f, 0.0f};

  for (WindowEvents::KeyPress& key_press : key_presses) {
    if (key_press.action == GLFW_PRESS || key_press.action == GLFW_REPEAT) {
      switch (key_press.key) {
        case GLFW_KEY_W:
          active_camera.eye += forward * move_speed;
          camera_updated = true;
          break;
        case GLFW_KEY_S:
          active_camera.eye -= forward * move_speed;
          camera_updated = true;
          break;
        case GLFW_KEY_A:
          active_camera.eye -= right * move_speed;
          camera_updated = true;
          break;
        case GLFW_KEY_D:
          active_camera.eye += right * move_speed;
          camera_updated = true;
          break;
        case GLFW_KEY_SPACE:
          active_camera.eye += up * move_speed;
          camera_updated = true;
          break;
        case GLFW_KEY_LEFT_SHIFT:
          active_camera.eye -= up * move_speed;
          camera_updated = true;
          break;
        case GLFW_KEY_ESCAPE:
        case GLFW_KEY_Q:
          return Result::RENDER_WINDOW_SHOULD_CLOSE;
      }
    }
  }

  if (camera_updated) {
    active_camera.update_view(); // Update view matrix once after all movements are processed
    render_events->events.push_back(RenderEvent::CAMERA_UPDATED);
    render_events->view = active_camera.view;
  }

  return Result::SUCCESS;
}

void Update::handle_mouse_movement(f32 xoffset, f32 yoffset, RenderEvents* render_events, ECS* ecs) {
  static const f32 sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  Components::Camera& active_camera = ecs->camera_components[active_player];

  active_camera.yaw += xoffset;
  active_camera.pitch -= yoffset;

  // Constrain the pitch so the screen doesn't flip
  if (active_camera.pitch > 89.0f)
    active_camera.pitch = 89.0f;
  if (active_camera.pitch < -89.0f)
    active_camera.pitch = -89.0f;

  active_camera.update_view();

  render_events->events.push_back(RenderEvent::CAMERA_UPDATED);
  render_events->view = active_camera.view;
}

void Update::load_chunks(ECS* ecs) {
  std::vector<glm::ivec3> active_chunk_corners = get_chunks_in_radius(ecs->pos_components[active_player].pos, 100.0f);

  for (const glm::ivec3& chunk_pos : active_chunk_corners) {
    if (!ecs->chunk_pos_index.contains(chunk_pos)) {
      EntityID new_chunk_id;
      ecs->create_entity_chunk(new_chunk_id);
      Components::Chunk& chunk = ecs->chunk_components[new_chunk_id];
      ChunkGenInfo gen_info = {
        chunk_pos
      };

      chunk.generate_cells(gen_info);
      ecs->chunk_pos_index[chunk_pos] = new_chunk_id;
      ecs->pos_components[new_chunk_id] = {static_cast<glm::vec3>(chunk_pos)};
    }
  }
}

std::vector<glm::ivec3> Update::get_chunks_in_radius(const glm::vec3& pos, f32 radius) {
  std::vector<glm::ivec3> corners;

  int minX = static_cast<s32>(floor((pos.x - radius) / Components::CHUNK_COMPONENT_CELL_WIDTH));
  int maxX = static_cast<s32>(floor((pos.x + radius) / Components::CHUNK_COMPONENT_CELL_WIDTH));
  int minY = static_cast<s32>(floor((pos.y - radius) / Components::CHUNK_COMPONENT_CELL_WIDTH));
  int maxY = static_cast<s32>(floor((pos.y + radius) / Components::CHUNK_COMPONENT_CELL_WIDTH));
  int minZ = static_cast<s32>(floor((pos.z - radius) / Components::CHUNK_COMPONENT_CELL_WIDTH));
  int maxZ = static_cast<s32>(floor((pos.z + radius) / Components::CHUNK_COMPONENT_CELL_WIDTH));

  for (int x = minX; x <= maxX; ++x) {
    for (int y = minY; y <= maxY; ++y) {
      for (int z = minZ; z <= maxZ; ++z) {
        glm::ivec3 corner = glm::ivec3(x, y, z) * static_cast<s32>(Components::CHUNK_COMPONENT_CELL_WIDTH);
        glm::vec3 delta = glm::vec3(corner) - pos;

        if (glm::length(delta) <= radius) {
          corners.push_back(corner);
        }
      }
    }
  }

  return corners;
}

