#ifndef ENTITY_H_
#define ENTITY_H_

#include "core.h"

#include <cstddef>

// Number of each component allowed
// Storage numbers bellow don't include pointer overhead for the maps
//
// A lot of these maxes aren't hard and won't actually effect anything
// at the moment

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
