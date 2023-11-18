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

Result ECS::create_entity(EntityID& id, ComponentBitmask component_bitmask) {
  Result get_entity_id_res = this->get_entity_id(id);
  if (get_entity_id_res != Result::SUCCESS) {
    return get_entity_id_res;
  }

  Result component_add_res;
  for (u8 i = 0; i < NUM_COMPONENT_TYPES; i++) {
    ComponentType component_type = static_cast<ComponentType>(1 << i);
    if (component_bitmask & component_type) {
      component_add_res = add_component_to_entity(id, component_type);
      if (component_add_res != Result::SUCCESS) {
        return component_add_res;
      }
    }
  }

  return Result::SUCCESS;
}

void ECS::destroy_entity(EntityID id) {
  ComponentBitmask component_bitmask = entities[id].components;

  // When chunks are a thing we also need to remove the id
  // from the chunk

  for (u8 i = 0; i < NUM_COMPONENT_TYPES; i++) {
    ComponentType component_type = static_cast<ComponentType>(1 << i);
    if (component_bitmask & component_type) {
      remove_component_from_entity(id, component_type);
    }
  }

  entity_id_pool.erase(id);
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
  
  LOG_WARN("Failed to get entity id. Pool full.");
  return Result::FAILURE_POOL_FULL;
}

