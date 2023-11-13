#ifndef ECS_H_
#define ECS_H_

#include "core.h"

#include "utils/math/vec.h"

#include <vector>

typedef u64 EntityID;

namespace Components {
  struct Kinetic {
    Vec3<f32> pos;
    Vec3<f32> vel;
    Vec3<f32> acc;
  };

  struct Camera {
    Vec3<f32> pos;
    Vec3<f32> rotation;
  };
}

class ECS {
public:
  static Result create(ECS*& ecs);
  static void destroy(ECS* ecs);

  Result create_player(EntityID* id);

private:
  // components
  std::vector<> kinetic;
  std::vector<>
}

#endif //ECS_H_
