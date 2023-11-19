#ifndef MESH_H_
#define MESH_H_

#include "core.h"

#include "glm/glm.hpp"

#include <vector>

struct Mesh {
  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
  };

  std::vector<Vertex> verticies;
  std::vector<u32> indicies;
};

#endif // MESH_H_
