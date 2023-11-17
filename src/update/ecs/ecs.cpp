#include "core.h"
#include "update/ecs/ecs.h"

#include <set>

Result ECS::create(ECS*& ecs) {
  ecs = new ECS;

  return Result::SUCCESS;
}

void ECS::destroy(ECS *ecs) {
  delete ecs;
}

Result ECS::create_entity(EntityID& id, ComponentBitmask) {
  return Result::SUCCESS;
}

Result ECS::get_entity_id(EntityID& id) {
  static EntityID current_entity = 1;
  static std::pair<std::set<EntityID>::iterator, bool> result;

  EntityID start_entity = current_entity;
  do {
    result = this->entity_id_pool.insert(current_entity);
    if(result.second) {
      id = current_entity;
      return Result::SUCCESS;
    }
    
    current_entity = (current_entity + 1) % MAX_TOTAL_ENTITIES;
    if (current_entity == 0) { ++current_entity; }  // Skip 0
  } while(current_entity != start_entity);

  return Result::FAILURE_POOL_FULL;
}

