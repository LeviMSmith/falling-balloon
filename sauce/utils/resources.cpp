#include "core.h"
#include "utils/resources.h"

#include <filesystem>

Result get_executable_path(std::filesystem::path& path);

Result get_resource_path(std::filesystem::path& path) {
  Result get_executable_path_res = get_executable_path(path);
  if (get_executable_path_res != Result::SUCCESS) {
    return get_executable_path_res;
  }
  path = path.parent_path() / RESOURCE_DIR;

  return Result::SUCCESS;
}

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
