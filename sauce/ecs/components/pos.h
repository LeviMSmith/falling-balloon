#ifndef POS_H_
#define POS_H_

#include "core.h"

#include "glm/glm.hpp"

namespace Components {
  struct Pos {
    Pos() : pos(glm::vec3(0.0f)) {}

    glm::vec3 pos;
  };
}

#endif // POS_H_
