#include "core.h"

#include "update/world/world.h"

Result World::create(World*& world) {
  world = new World;

  return Result::SUCCESS;
}

void World::destroy(World* world) {
  delete world;
}
