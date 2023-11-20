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
  for (WindowEvent event : window_events->events) {
    switch (event) {
      default:
        break;
    }
  }

  if (window_events->key_presses.size() > 0) {
    handle_keypresses(window_events->key_presses, render_events, ecs);
  }

  return Result::SUCCESS;
}

void Update::handle_keypresses(std::vector<WindowEvents::KeyPress>& key_presses, RenderEvents* render_events, ECS* ecs) {
  static f32 roation_increment = glm::radians(1.0f);
  b8 camera_updated = false;
  glm::vec3 direction;
  Components::Camera& active_camera = ecs->camera_components[active_player];
  for (WindowEvents::KeyPress& key_press : key_presses) {
    if (key_press.action == GLFW_PRESS || key_press.action == GLFW_REPEAT) {
      switch (key_press.key) {
        case GLFW_KEY_A: {
          direction.x = cos(roation_increment);
          direction.z = cos(roation_increment);
          active_camera.direction -= direction;
          active_camera.update_view();
          camera_updated = true;
          break;
        }
        case GLFW_KEY_D: {
          direction.x = cos(roation_increment);
          direction.z = cos(roation_increment);
          active_camera.direction += direction;
          active_camera.update_view();
          camera_updated = true;
          break;
        }
        case GLFW_KEY_W: {
          direction.y = cos(roation_increment);
          direction.z = cos(roation_increment);
          active_camera.direction += direction;
          active_camera.update_view();
          camera_updated = true;
          break;
        }
        case GLFW_KEY_S: {
          direction.y = cos(roation_increment);
          direction.z = cos(roation_increment);
          active_camera.direction -= direction;
          active_camera.update_view();
          camera_updated = true;
          break;
        }
        default:
          break;
      }
    }
  }

  char direction_str[1024];
  sprintf(direction_str, "%f %f %f", active_camera.direction.x, active_camera.direction.y, active_camera.direction.z);
  LOG_DEBUG(direction_str);
  
  if (camera_updated) {
    render_events->events.push_back(RenderEvent::CAMERA_UPDATED);
  }
}
