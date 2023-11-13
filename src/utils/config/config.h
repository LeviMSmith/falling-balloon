#ifndef CONFIG_H_
#define CONFIG_H_

#include "core.h"

class Config {
public:
  static Result create(Config*& config);
  static void destroy(Config* config);
  Result save(const Config* config, const char* filepath);
  Result load(Config* config, const char* filepath);

  u32 window_width;
  u32 window_height;
  b8 window_start_maximixed;
};

#endif // CONFIG_H_
