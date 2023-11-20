#ifndef LOG_H
#define LOG_H

#include "utils/result.h"

enum class LogLevel {
  DEBUG = 0,
  INFO,
  WARN,
  ERROR,
  FATAL,
};

Result init_log(LogLevel level);

void app_log(LogLevel level, const char* msg, ...);

#define LOG_DEBUG(...) app_log(LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) app_log(LogLevel::INFO, __VA_ARGS__)
#define LOG_WARN(...) app_log(LogLevel::WARN, __VA_ARGS__)
#define LOG_ERROR(...) app_log(LogLevel::ERROR, __VA_ARGS__)
#define LOG_FATAL(...) app_log(LogLevel::FATAL, __VA_ARGS__)

#endif // LOG_H
