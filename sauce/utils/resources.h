#ifndef RESOURCES_H_
#define RESOURCES_H_

#include "core.h"

#include <filesystem>
#include <vector>

class Resources {
public:
  static Result get_resource_path(std::filesystem::path& path);

  static Result load_image(std::vector<u8>& data, int& width, int& height, int& nrChannels, const std::filesystem::path& path);

  static const char* SHADER_PATH;
};

#endif // RESOURCES_H_
