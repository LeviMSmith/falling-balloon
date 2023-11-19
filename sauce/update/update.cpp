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

Result Update::update(ECS* ecs, RenderEvents* render_events) {
  return Result::SUCCESS;
}
