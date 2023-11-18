#include "core.h"

#include "update/ecs/ecs.h"

#include "update/components/camera.h"
#include "update/components/chunk.h"
#include "update/components/graphics_pipline.h"
#include "update/components/kinetic.h"
#include "update/components/pos.h"

#include <set>

Result ECS::create(ECS*& ecs) {
  ecs = new ECS;

  return Result::SUCCESS;
}

void ECS::destroy(ECS* ecs) {
  if (ecs != nullptr) {
    for (auto it = ecs->entity_id_pool.begin(); it != ecs->entity_id_pool.end(); ) {
      EntityID current_id = *it;
      ++it;  // Increment iterator before the potential deletion
      ecs->destroy_entity(current_id);
    }

    delete ecs;
  }
}

Result ECS::create_entity(EntityID& entity_id, ComponentBitmask component_bitmask) {
  Result get_entity_id_res = this->get_entity_id(entity_id);
  if (get_entity_id_res != Result::SUCCESS) {
    return get_entity_id_res;
  }

  Result component_add_res;
  for (u8 i = 0; i < NUM_COMPONENT_TYPES; i++) {
    ComponentType component_type = static_cast<ComponentType>(1 << i);
    if (component_bitmask & component_type) {
      component_add_res = add_component_to_entity(entity_id, component_type);
      if (component_add_res != Result::SUCCESS) {
        return component_add_res;
      }
    }
  }

  return Result::SUCCESS;
}

void ECS::destroy_entity(EntityID entity_id) {
  ComponentBitmask component_bitmask = entities[entity_id].components;

  // When chunks are a thing we also need to remove the id
  // from the chunk

  for (u8 i = 0; i < NUM_COMPONENT_TYPES; i++) {
    ComponentType component_type = static_cast<ComponentType>(1 << i);
    if (component_bitmask & component_type) {
      remove_component_from_entity(entity_id, component_type);
    }
  }

  entity_id_pool.erase(entity_id);
}

Result ECS::add_component_to_entity(EntityID entity_id, ComponentType component_type) {
  Result component_create_res;
  switch (component_type) {
    case ComponentType::POS: {
      Components::Pos& pos = pos_components[entity_id];
      component_create_res = Components::Pos::create(pos);
      break;
    }
    case ComponentType::KINETIC: {
      Components::Kinetic& kinetic = kinetic_components[entity_id];
      component_create_res = Components::Kinetic::create(kinetic);
      break;
    }
    case ComponentType::CAMERA: {
      Components::Camera& camera = camera_components[entity_id];
      component_create_res = Components::Camera::create(camera);
      break;
    }
    case ComponentType::GRAPHICS_PIPELINE: {
      Components::GraphicsPipeline& graphics_pipeline = graphics_pipeline_components[entity_id];
      component_create_res = Components::GraphicsPipeline::create(graphics_pipeline);
      break;
    }
    case ComponentType::CHUNK: {
      Components::Chunk& chunk = chunk_components[entity_id];
      component_create_res = Components::Chunk::create(chunk);
      break;
    }
    default: {
      LOG_WARN("Got weird component type in entity component addition");
      return Result::FAILURE_UNKNOWN_COMPONENT_TYPE;
    }
  }

  if (component_create_res == Result::SUCCESS) {
    entities[entity_id].components = entities[entity_id].components | component_type;
  }

  return component_create_res;
}

void ECS::remove_component_from_entity(EntityID entity_id, ComponentType component_type) {
  switch (component_type) {
    case ComponentType::POS: {
      Components::Pos& pos = pos_components[entity_id];
      Components::Pos::destroy(pos);
      break;
    }
    case ComponentType::KINETIC: {
      Components::Kinetic& kinetic = kinetic_components[entity_id];
      Components::Kinetic::destroy(kinetic);
      break;
    }
    case ComponentType::CAMERA: {
      Components::Camera& camera = camera_components[entity_id];
      Components::Camera::destroy(camera);
      break;
    }
    case ComponentType::GRAPHICS_PIPELINE: {
      Components::GraphicsPipeline& graphics_pipeline = graphics_pipeline_components[entity_id];
      Components::GraphicsPipeline::destroy(graphics_pipeline);
      break;
    }
    case ComponentType::CHUNK: {
      Components::Chunk& chunk = chunk_components[entity_id];
      Components::Chunk::destroy(chunk);
      break;
    }
    default: {
      LOG_WARN("Got weird component type in entity component removal");
    }
  }

  entities[entity_id].components = entities[entity_id].components & (~component_type);
}

Result ECS::create_entity_player(EntityID& entity_id) {
  constexpr ComponentBitmask player_component_bitmask = ComponentType::CHUNK | ComponentType::KINETIC | ComponentType::CAMERA;

  return create_entity(entity_id, player_component_bitmask);
}

Result ECS::create_entity_chunk(EntityID& entity_id) {
  constexpr ComponentBitmask chunk_component_bitmask = ComponentType::POS | ComponentType::CHUNK;

  return create_entity(entity_id, chunk_component_bitmask);
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
