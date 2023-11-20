#ifndef MESH_H_
#define MESH_H_

#include "core.h"

#include "glm/glm.hpp"

#include <vector>

struct Mesh {
  struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
  };

  std::vector<Vertex> vertices;
  glm::mat4 model;
};

#endif // MESH_H_
