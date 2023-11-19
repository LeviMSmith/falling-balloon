#include "core.h"
#include "utils/math/dim.h"

namespace Dim {
  template<typename Index, typename Coordinate>
  inline Index threed_to_oned(Coordinate x, Coordinate y, Coordinate z, Coordinate width, Coordinate height) {
    return x + y * width + z * width * height;
  }
}
