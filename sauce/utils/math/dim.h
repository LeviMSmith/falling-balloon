#ifndef DIM_H_
#define DIM_H_

#include "core.h"

namespace Dim {
  template<typename Index, typename Coordinate>
  Index threed_to_oned(Coordinate x, Coordinate y, Coordinate z, Coordinate width, Coordinate height);
}

#endif // DIM_H_
