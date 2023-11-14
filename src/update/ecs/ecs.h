#ifndef ECS_H_
#define ECS_H_

#include "core.h"

#include "utils/math/vec.h"

#include <vector>

typedef u32 EntityID;

// Number of each component allowed (and preallocated)
//
// NOTE: Components that have the same max don't need an index because they can be
// looked up directly in the array.
//
// NOTE: The indexs are very quick for lookup times, but much larger than the components
// themselves in terms of memory. Maybe either fewer entities or a different lookup system.
//
#define FB_ECS_MAX_ENTITY_PER_CHUNK 8192 // x4 32.8KB
#define FB_ECS_MAX_CHUNK 20000 // x32768 655.4MB
#define FB_ECS_MAX_POS 72000000 // x12 864MB
#define FB_ECS_MAX_KINETIC 65536 // x36 2.3MB + Index (72000000 * 2 = 144MB) 
#define FB_ECS_MAX_CAMERA 256 // x24 6144B + Index (72000000 * 1 = 72MB)
#define FB_ECS_MAX_GRAPHICS_PIPELINE 72000000 // x1 72MB
#define FB_ECS_MAX_BLOCK_TYPE 72000000 // x1 72MB

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

  // Only allowed on entities with corresponding GraphicsPipeline
  struct BlockType {
    u8 block;
  };

  struct Chunk {
    EntityID entities[FB_ECS_MAX_ENTITY_PER_CHUNK];
  };
}

class ECS {
public:
  static Result create(ECS*& ecs);
  static void destroy(ECS* ecs);

  // Factory functions
  Result create_player(EntityID* id);
  Result create_block(EntityID* id);

private:
  // components and their (optional) index
  Components::Pos poss[FB_ECS_MAX_POS];
  Components::Kinetic kinetics[FB_ECS_MAX_KINETIC];
  Components::Camera cameras[FB_ECS_MAX_CAMERA];
  Components::GraphicsPipeline graphics_pipelines[FB_ECS_MAX_GRAPHICS_PIPELINE];
  Components::BlockType block_types[FB_ECS_MAX_BLOCK_TYPE];
  Components::Chunk chunks[FB_ECS_MAX_CHUNK];
};

#endif //ECS_H_
