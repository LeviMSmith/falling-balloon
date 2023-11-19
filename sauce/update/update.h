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

  Result update(ECS* ecs, RenderEvents* render_events);
};

#endif // UPDATE_H_
