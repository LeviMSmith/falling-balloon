#include <stdio.h>
#include <time.h>

#include "utils/result.h"
#include "utils/log.h"

const long CLOCKS_PER_MILLISEC = CLOCKS_PER_SEC / 1000;

LogLevel log_level_threshold = LogLevel::INFO;

Result init_log(LogLevel level) {
  log_level_threshold = level;

  return RESULT_TYPE_SUCCESS;
}

const char* log_level_names[] = {
  "DEBUG",
  "INFO",
  "WARN",
  "ERROR",
  "FATAL"
};

const char* log_level_colors[] = {
  "\033[94m", // Bright Blue
  "\033[32m", // Dark Green
  "\033[93m", // Bright Yellow
  "\033[31m", // Dark Red
  "\033[35m" // Dark Magenta
};

void app_log(const char* msg, LogLevel level) {
  if (level >= log_level_threshold) {
    printf("[%ld] [%s%s\033[0m] %s\n", clock()/CLOCKS_PER_MILLISEC, log_level_colors[level], log_level_names[level], msg);
  }
}
