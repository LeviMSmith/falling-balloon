#ifndef ECS_H_
#define ECS_H_

// NOTE: Components can come from anywhere, but things
// not specific to one module are included in the default
// ecs component folder.

#include "core.h"

#include "utils/math/vec.h"

#include <vector>
#include <map>

typedef u32 EntityID;
typedef u32 ComponentBitmask;

enum ComponentType: u32 {
    POS = 1 << 0,
    KINETIC = 1 << 1,
    CAMERA = 1 << 2,
    GRAPHICS_PIPELINE = 1 << 3,
    CHUNK = 1 << 4
};

// 8bytes
struct Entity {
  EntityID id;
  ComponentBitmask components;
};

// Number of each component allowed (and preallocated)
// Storage numbers bellow don't include pointer overhead for the maps

constexpr size_t MAX_GLOBAL_ENTITIES = 4096;
constexpr size_t MAX_CHUNK_ENTITIES = 2048;
constexpr size_t MAX_CHUNKS = 20000;

// 40964096 Entiies in this case
constexpr size_t MAX_TOTAL_ENTITIES = (MAX_CHUNKS * MAX_CHUNK_ENTITIES) + MAX_GLOBAL_ENTITIES;

constexpr size_t MAX_POS_COMPONENTS = MAX_TOTAL_ENTITIES; // x12 bytes 491MB
constexpr size_t MAX_KINETIC_COMPONENTS = MAX_TOTAL_ENTITIES / 2; // x24 bytes 491MB
constexpr size_t MAX_CAMERA_COMPONENTS = 24; // x24 bytes 576B
// Graphics backend makes these and they are likely large, but not preallocated
// like the other components and probably won't ever reach this max.
constexpr size_t MAX_GRAPHICS_PIPELINE_COMPONENTS = 100;
constexpr size_t MAX_CHUNK_COMPONENTS = MAX_CHUNKS; // x69640 1393MB

constexpr size_t CHUNK_COMPONENT_CELL_WIDTH = 16;

namespace Components {
  struct Pos {
    Vec3<f32> pos;
  };

  struct Kinetic {
    Vec3<f32> vel;
    Vec3<f32> acc;
  };

  struct Camera {
    Vec3<f32> pos;
    Vec3<f32> rotation;
  };

  struct GraphicsPipeline {
    u8 type;
  };

  struct Chunk {
    std::vector<Entity> entities;
    u8 cells[CHUNK_COMPONENT_CELL_WIDTH * CHUNK_COMPONENT_CELL_WIDTH * CHUNK_COMPONENT_CELL_WIDTH];
  };
}

class ECS {
public:
  static Result create(ECS*& ecs);
  static void destroy(ECS* ecs);

  void destroy_entity(Entity* entity);

  // Factory functions
  Result create_entity_player(Entity*& entity);
  Result create_entity_chunk(Entity*& entity);

private:
  // Components
  std::map<EntityID, Components::Pos> pos_components;
  std::map<EntityID, Components::Kinetic> kinetic_components;
  std::map<EntityID, Components::Camera> camera_components;
  std::map<EntityID, Components::GraphicsPipeline> graphics_pipeline_components;
  std::map<EntityID, Components::Chunk> chunk_components;
};

#endif //ECS_H_
