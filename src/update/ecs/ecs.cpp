#include "core.h"
#include "update/ecs/ecs.h"

Result ECS::create(ECS*& ecs) {
  ecs = new ECS;

  return RESULT_TYPE_SUCCESS;
}

void ECS::destroy(ECS *ecs) {
  delete ecs;
}

Result ECS::create_entity(Entity* entity, ComponentType component_type) {
  return RESULT_TYPE_SUCCESS;
}
