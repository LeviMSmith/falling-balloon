#ifndef UPDATE_H_
#define UPDATE_H_

#include "core.h"

#include "update/ecs/ecs.h"

class Update {
public:
  static Result create(Update*& update);
  static void destroy(Update* update);

private:
  ECS* ecs;

  EntityID active_player;
};

#endif // UPDATE_H_
