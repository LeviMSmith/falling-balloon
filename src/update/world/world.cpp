#include "core.h"

#include "update/world/world.h"

Result World::create(World*& world) {
  return RESULT_TYPE_SUCCESS;
}

void World::destroy(World* world) {
  delete world;
}
