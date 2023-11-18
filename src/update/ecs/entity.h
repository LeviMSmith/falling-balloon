#ifndef ENTITY_H_
#define ENTITY_H_

#include "core.h"

typedef u32 EntityID;
typedef u32 ComponentBitmask;

enum ComponentType: u32 {
    POS = 1 << 0,
    KINETIC = 1 << 1,
    CAMERA = 1 << 2,
    GRAPHICS_PIPELINE = 1 << 3,
    CHUNK = 1 << 4
};

constexpr u8 NUM_COMPONENT_TYPES = 5;

struct Entity {
  ComponentBitmask components;
};

#endif // ENTITY_H_
