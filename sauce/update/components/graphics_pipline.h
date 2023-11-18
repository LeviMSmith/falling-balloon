#ifndef GRAPHICS_PIPELINE_H_
#define GRAPHICS_PIPELINE_H_

#include "core.h"

namespace Components {
  class GraphicsPipeline {
  public:
    u8 type;

    static Result create(GraphicsPipeline& graphics_pipline);
    static void destroy(GraphicsPipeline& graphics_pipline);
  };
}

#endif // GRAPHICS_PIPELINE_H_
