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
#include <set>
#include <vector>

// Number of each component allowed (and preallocated)
// Storage numbers bellow don't include pointer overhead for the maps

constexpr size_t MAX_CHUNK_ENTITIES = 2048;
constexpr size_t MAX_CHUNKS = 20000;
constexpr size_t MAX_GLOBAL_ENTITIES = MAX_CHUNKS + 4096;

// 40964096 Entiies in this case
constexpr size_t MAX_TOTAL_NON_CHUNK_ENTITIES =
    (MAX_CHUNKS * MAX_CHUNK_ENTITIES) + MAX_GLOBAL_ENTITIES - MAX_CHUNKS;
constexpr size_t MAX_TOTAL_ENTITIES =
    (MAX_CHUNKS * MAX_CHUNK_ENTITIES) + MAX_GLOBAL_ENTITIES;

constexpr size_t MAX_POS_COMPONENTS =
    MAX_TOTAL_NON_CHUNK_ENTITIES; // x12 bytes 491MB
constexpr size_t MAX_KINETIC_COMPONENTS =
    MAX_TOTAL_NON_CHUNK_ENTITIES / 2;        // x24 bytes 491MB
constexpr size_t MAX_CAMERA_COMPONENTS = 24; // x24 bytes 576B
// Graphics backend makes these and they are likely large, but not preallocated
// like the other components and probably won't ever reach this max.
constexpr size_t MAX_GRAPHICS_PIPELINE_COMPONENTS = 100;
constexpr size_t MAX_CHUNK_COMPONENTS = MAX_CHUNKS; // x69640 1393MB

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
  Result create_entity_player(EntityID& id);
  Result create_entity_chunk(EntityID& id);

private:
  Entity entities[MAX_TOTAL_ENTITIES];
  std::set<EntityID> entity_id_pool;

  Result get_entity_id(EntityID& id);

  // Components
  std::map<EntityID, Components::Pos> pos_components;
  std::map<EntityID, Components::Kinetic> kinetic_components;
  std::map<EntityID, Components::Camera> camera_components;
  std::map<EntityID, Components::GraphicsPipeline> graphics_pipeline_components;
  std::map<EntityID, Components::Chunk> chunk_components;
};

#endif // ECS_H_
