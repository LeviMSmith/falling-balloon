#ifndef CONFIG_H_
#define CONFIG_H_

#include "core.h"

class Config {
public:
  static Result create(Config*& config);
  static void destroy(Config* config);
  Result save(const char* filepath) const;
  Result load(const char* filepath);

  u32 window_width;
  u32 window_height;
  b8 window_start_maximixed;
  u16 fps_cap;
};

#endif // CONFIG_H_
