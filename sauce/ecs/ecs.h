#ifndef ECS_H_
#define ECS_H_

#include "core.h"

#include "utils/data-structures/lru-cache.h"

#include "ecs/entity.h"

#include "ecs/components/camera.h"
#include "ecs/components/chunk.h"
#include "ecs/components/graphics_pipline.h"
#include "ecs/components/kinetic.h"
#include "ecs/components/pos.h"

#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>

class ECS {
public:
  ECS() : mesh_components(LRUcache<EntityID, Mesh>(MAX_MESH_COMPONENTS)) {}

  static Result create(ECS*& ecs);
  static void destroy(ECS* ecs);

  Result create_entity(EntityID& entity_id, ComponentBitmask component_bitmask = 0);
  void destroy_entity(EntityID entity_id);

  Result add_component_to_entity(EntityID entity_id, ComponentType component_type);
  void remove_component_from_entity(EntityID entity,
                                      ComponentType component_type);

  std::vector<Mesh> get_chunk_mesh_component_batch(const std::vector<EntityID>& entity_ids);

  // Factory functions
  Result create_entity_player(EntityID& entity_id);
  Result create_entity_chunk(EntityID& entity_id);
private:
  Entity entities[MAX_TOTAL_ENTITIES];
  std::unordered_set<EntityID> entity_id_pool;

  Result get_entity_id(EntityID& id);

  // Components
  // With serialization, these should be turned to caches
  // and flush to disk when overrun.
  std::unordered_map<EntityID, Components::Pos> pos_components;
  std::unordered_map<EntityID, Components::Kinetic> kinetic_components;
  std::unordered_map<EntityID, Components::Camera> camera_components;
  std::unordered_map<EntityID, Components::GraphicsPipeline> graphics_pipeline_components;
  std::unordered_map<EntityID, Components::Chunk> chunk_components;
  LRUcache<EntityID, Mesh> mesh_components;

  friend class Render;
  friend class Update;
};

#endif // ECS_H_
