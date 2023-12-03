#include "core.h"
#include "update/update.h"

#include "ecs/ecs.h"

Result Update::create(Update*& update, ECS* ecs) {
  update = new Update;
  Result player_create_res = ecs->create_entity_player(update->active_player);
  if (player_create_res != Result::SUCCESS) {
    return player_create_res;
  }

  EntityID chunk_id;
  ecs->create_entity_chunk(chunk_id);

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
  static const f32 sensitivity = 0.1f; // Adjust as needed
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
