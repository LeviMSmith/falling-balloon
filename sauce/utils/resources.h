#ifndef RESOURCES_H_
#define RESOURCES_H_

#include "core.h"

#include <filesystem>

class Resources {
public:
  static Result get_resource_path(std::filesystem::path& path);

  static const char* SHADER_PATH;
};

#endif // RESOURCES_H_
