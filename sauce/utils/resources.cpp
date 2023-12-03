#include "core.h"
#include "utils/resources.h"

#include <cstring>
#include <filesystem>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Result get_executable_path(std::filesystem::path& path);

Result Resources::get_resource_path(std::filesystem::path& path) {
  Result get_executable_path_res = get_executable_path(path);
  if (get_executable_path_res != Result::SUCCESS) {
    return get_executable_path_res;
  }
  path = path.parent_path() / RESOURCE_DIR;

  return Result::SUCCESS;
}

const char* Resources::SHADER_PATH = "shaders";

#ifdef PLATFORM_LINUX
Result get_executable_path(std::filesystem::path& path) {
  path = std::filesystem::read_symlink("/proc/self/exe");
  return Result::SUCCESS;
}
#else
Result get_executable_path(std::filesystem::path& path) {
  LOG_WARN("get_executable_path was called on an unsupported platform!");
  return Result::FAILURE_PLATFORM_ISSUE;
}
#endif

Result Resources::load_image(std::vector<u8>& data, int& width, int& height, int& nrChannels, const std::filesystem::path& path) {
    u8* raw_data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0); // Ensure path is converted to string if needed
    if (!raw_data) {
        return Result::FAILURE_FILE_IO;       
    } 

    size_t dataSize = width * height * nrChannels;
    data.resize(dataSize);
    std::memcpy(data.data(), raw_data, dataSize);

    stbi_image_free(raw_data);

    return Result::SUCCESS;
}

