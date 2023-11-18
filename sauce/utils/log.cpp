#include <stdio.h>
#include <chrono>

#include "utils/result.h"
#include "utils/log.h"

LogLevel log_level_threshold = LogLevel::INFO;

Result init_log(LogLevel level) {
  log_level_threshold = level;

  return Result::SUCCESS;
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
    static auto start_time = std::chrono::steady_clock::now(); // Record start time

    if (level >= log_level_threshold) {
        auto current_time = std::chrono::steady_clock::now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();

        printf("[%ld ms] [%s%s\033[0m] %s\n", time_elapsed, log_level_colors[level], log_level_names[level], msg);
    }
}


