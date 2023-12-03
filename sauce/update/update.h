#ifndef UPDATE_H_
#define UPDATE_H_

#include "core.h"

#include "event/event.h"
#include "ecs/ecs.h"

class Update {
public:
  static Result create(Update*& update, ECS* ecs);
  static void destroy(Update* update);

  EntityID active_player;

  Result update(WindowEvents* window_events, RenderEvents* render_events, ECS* ecs);
private:
  Result handle_keypresses(std::vector<WindowEvents::KeyPress>& key_presses, RenderEvents* render_events, ECS* ecs);
  void handle_mouse_movement(f32 xoffset, f32 yoffset, RenderEvents* render_events, ECS* ecs);
};

#endif // UPDATE_H_
