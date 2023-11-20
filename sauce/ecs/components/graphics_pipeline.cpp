#include "core.h"
#include "ecs/components/graphics_pipline.h"

namespace Components {
  Result GraphicsPipeline::create(GraphicsPipeline& graphics_pipline) {
    graphics_pipline.type = 0;

    return Result::SUCCESS;
  }

  void GraphicsPipeline::destroy(GraphicsPipeline& graphics_pipline) {}
}
