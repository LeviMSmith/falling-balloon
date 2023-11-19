#include "core.h"
#include "update/update.h"

#include "ecs/ecs.h"

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
