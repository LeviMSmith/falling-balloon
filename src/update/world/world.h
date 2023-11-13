#ifndef WORLD_H_
#define WORLD_H_

#include "core.h"

class World {
public:
  static Result create(World*& world);
  static void destroy(World*);
};

#endif // WORLD_H_
