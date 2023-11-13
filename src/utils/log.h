#ifndef LOG_H
#define LOG_H

#include "utils/result.h"

enum LogLevel {
  DEBUG = 0,
  INFO,
  WARN,
  ERROR,
  FATAL,
};

Result init_log(LogLevel level);

void app_log(const char* msg, LogLevel level);

#define LOG_DEBUG(...) app_log(__VA_ARGS__, LogLevel::DEBUG)
#define LOG_INFO(...) app_log(__VA_ARGS__, LogLevel::INFO)
#define LOG_WARN(...) app_log(__VA_ARGS__, LogLevel::WARN)
#define LOG_ERROR(...) app_log(__VA_ARGS__, LogLevel::ERROR)
#define LOG_FATAL(...) app_log(__VA_ARGS__, LogLevel::FATAL)

#endif // LOG_H
