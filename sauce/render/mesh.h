#ifndef MESH_H_
#define MESH_H_

#include "core.h"

#include "GL/glew.h"
#include "glm/glm.hpp"

#include <vector>

struct Mesh {
  struct Vertex {
    glm::vec3 position;
    GLuint texnorm;
  };

  std::vector<Vertex> vertices;
  glm::mat4 model;
};

#endif // MESH_H_
