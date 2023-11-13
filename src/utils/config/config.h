#ifndef CONFIG_H
#define CONFIG_H

#include "core.h"

struct Config {
  u32 window_width;
  u32 window_height;
  b8 window_start_maximixed;
};

void config_default(Config* config);
Result config_save(const Config* config, const char* filepath);
Result config_load(Config* config, const char* filepath);

#endif
