#ifndef ECS_H_
#define ECS_H_

#include "core.h"

#include "utils/math/vec.h"

#include "update/ecs/entity.h"

#include "update/components/camera.h"
#include "update/components/chunk.h"
#include "update/components/graphics_pipline.h"
#include "update/components/kinetic.h"
#include "update/components/pos.h"

#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>

class ECS {
public:
  static Result create(ECS*& ecs);
  static void destroy(ECS* ecs);

  Result create_entity(EntityID& entity_id, ComponentBitmask component_bitmask = 0);
  void destroy_entity(EntityID entity_id);

  Result add_component_to_entity(EntityID entity_id, ComponentType component_type);
  void remove_component_from_entity(EntityID entity,
                                      ComponentType component_type);

  // Factory functions
  Result create_entity_player(EntityID& entity_id);
  Result create_entity_chunk(EntityID& entity_id);

private:
  Entity entities[MAX_TOTAL_ENTITIES];
  std::unordered_set<EntityID> entity_id_pool;

  Result get_entity_id(EntityID& id);

  // Components
  std::unordered_map<EntityID, Components::Pos> pos_components;
  std::unordered_map<EntityID, Components::Kinetic> kinetic_components;
  std::unordered_map<EntityID, Components::Camera> camera_components;
  std::unordered_map<EntityID, Components::GraphicsPipeline> graphics_pipeline_components;
  std::unordered_map<EntityID, Components::Chunk> chunk_components;

  friend class Render;
  friend class Update;
};

#endif // ECS_H_
