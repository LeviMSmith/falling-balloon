#include "core.h"

#include "ecs/ecs.h"

#include "ecs/components/camera.h"
#include "ecs/components/chunk.h"
#include "ecs/components/graphics_pipline.h"
#include "ecs/components/kinetic.h"
#include "ecs/components/pos.h"

#include "utils/threadpool.h"

#include <set>
#include <vector>
#include <unordered_map>
#include <future>
#include <functional>

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
      pos_components[entity_id] = Components::Pos();
      component_create_res = Result::SUCCESS;
      break;
    }
    case ComponentType::KINETIC: {
      Components::Kinetic& kinetic = kinetic_components[entity_id];
      component_create_res = Components::Kinetic::create(kinetic);
      break;
    }
    case ComponentType::CAMERA: {
      camera_components[entity_id] = Components::Camera();
      component_create_res = Result::SUCCESS;
      break;
    }
    case ComponentType::GRAPHICS_PIPELINE: {
      Components::GraphicsPipeline& graphics_pipeline = graphics_pipeline_components[entity_id];
      component_create_res = Components::GraphicsPipeline::create(graphics_pipeline);
      break;
    }
    case ComponentType::CHUNK: {
      chunk_components[entity_id] = Components::Chunk();
      component_create_res = Result::SUCCESS;
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
      break;
    }
    case ComponentType::KINETIC: {
      Components::Kinetic& kinetic = kinetic_components[entity_id];
      Components::Kinetic::destroy(kinetic);
      break;
    }
    case ComponentType::CAMERA: {
      break;
    }
    case ComponentType::GRAPHICS_PIPELINE: {
      Components::GraphicsPipeline& graphics_pipeline = graphics_pipeline_components[entity_id];
      Components::GraphicsPipeline::destroy(graphics_pipeline);
      break;
    }
    case ComponentType::CHUNK: {
      break;
    }
    default: {
      LOG_WARN("Got weird component type in entity component removal: %u", (u32)component_type);
    }
  }

  entities[entity_id].components = entities[entity_id].components & (~component_type);
}

Result ECS::create_entity_player(EntityID& entity_id) {
  constexpr ComponentBitmask player_component_bitmask = ComponentType::KINETIC | ComponentType::CAMERA | ComponentType::POS;

  return create_entity(entity_id, player_component_bitmask);
}

Result ECS::create_entity_chunk(EntityID& entity_id) {
  constexpr ComponentBitmask chunk_component_bitmask = ComponentType::POS | ComponentType::CHUNK | ComponentType::MESH;

  Result create_res = create_entity(entity_id, chunk_component_bitmask);

  return create_res;
}

std::vector<Mesh> ECS::get_chunk_mesh_component_batch(const std::vector<EntityID>& entity_ids) {
  std::vector<Mesh> return_meshes;

  Mesh mesh;
  std::unordered_map<EntityID, std::future<Mesh>> mesh_futures;

  for (EntityID entity_id : entity_ids) {
    if (mesh_components.get(entity_id, mesh)) {
      return_meshes.push_back(mesh);
    }
    else {
      Components::Chunk& chunk = chunk_components.at(entity_id);
      Components::Pos chunk_pos = pos_components.at(entity_id);
      LOG_DEBUG("Chun pos %f, %f, %f", chunk_pos.pos.x, chunk_pos.pos.y, chunk_pos.pos.z);
      auto task = [&chunk, chunk_pos] { return chunk.generate_mesh(chunk_pos.pos); };
      mesh_futures[entity_id] = ThreadPool::enqueue(task);
    }
  }

  for (auto& mesh_future : mesh_futures) {
    mesh = mesh_future.second.get();
    return_meshes.push_back(mesh);
    mesh_components.put(mesh_future.first, mesh);
  }

  return return_meshes;
}

Result ECS::get_entity_id(EntityID& id) {
  static EntityID current_entity = 1;
  static std::pair<std::unordered_set<EntityID>::iterator, bool> result;

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

