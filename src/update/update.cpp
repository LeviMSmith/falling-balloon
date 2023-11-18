#include "core.h"
#include "update/update.h"

#include "update/ecs/ecs.h"

Result Update::create(Update*& update) {
  update = new Update;

  Result ecs_create_res = ECS::create(update->ecs);
  if (ecs_create_res != Result::SUCCESS) {
    Update::destroy(update);
    return ecs_create_res; 
  }

  Result player_create_res = update->ecs->create_entity_player(update->active_player);
  if (player_create_res != Result::SUCCESS) {
    return player_create_res;
  }

  return Result::SUCCESS;
}

void Update::destroy(Update* update) {
  ECS::destroy(update->ecs);
  if (update != nullptr) {
    delete update;
  }
}
