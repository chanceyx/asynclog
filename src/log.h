#pragma once

#include "log_event.h"

// Stream style log.
// Usage:
//  LOG_AT_LEVEL(logger, asynclog::LogLevel::FATAL) << "fatal info";
//  LOG_WARN(logger) << "error info";
#define LOG_AT_LEVEL(logger, level)                                          \
  if (logger->getLevel() <= level)                                           \
  asynclog::LogEventWrapper(                                                 \
      std::shared_ptr<asynclog::LogEvent>(                                   \
          new asynclog::LogEvent(logger, level, __FILE__, __LINE__, 0, 1, 2, \
                                 time(0), "test_thread")))                   \
      .getEventStirngStream()

#define LOG_DEBUG(logger) LOG_AT_LEVEL(logger, asynclog::LogLevel::DEBUG)

#define LOG_INFO(logger) LOG_AT_LEVEL(logger, asynclog::LogLevel::INFO)

#define LOG_WARN(logger) LOG_AT_LEVEL(logger, asynclog::LogLevel::WARN)

#define LOG_ERROR(logger) LOG_AT_LEVEL(logger, asynclog::LogLevel::ERROR)

#define LOG_FATAL(logger) LOG_AT_LEVEL(logger, asynclog::LogLevel::FATAL)

// Format style log.
// Usage:
//  LOG_AT_LEVELF(logger, asynclog::LogLevel::FATAL, "%s - errno: %d",
//               "fatal info", errno);
//  LOG_ERRORF(logger, "%s - errno: %d", "error info", errno);
#define LOG_AT_LEVELF(logger, level, fmt, ...)                               \
  if (logger->getLevel() <= level)                                           \
  asynclog::LogEventWrapper(                                                 \
      std::shared_ptr<asynclog::LogEvent>(                                   \
          new asynclog::LogEvent(logger, level, __FILE__, __LINE__, 0, 1, 2, \
                                 time(0), "test_thread")))                   \
      .getEvent()                                                            \
      ->format(fmt, __VA_ARGS__)

#define LOG_DEBUGF(logger, fmt, ...) \
  LOG_AT_LEVELF(logger, asynclog::LogLevel::DEBUG, fmt, __VA_ARGS__)

#define LOG_INFOF(logger, fmt, ...) \
  LOG_AT_LEVELF(logger, asynclog::LogLevel::INFO, fmt, __VA_ARGS__)

#define LOG_WARNF(logger, fmt, ...) \
  LOG_AT_LEVELF(logger, asynclog::LogLevel::WARN, fmt, __VA_ARGS__)

#define LOG_ERRORF(logger, fmt, ...) \
  LOG_AT_LEVELF(logger, asynclog::LogLevel::ERROR, fmt, __VA_ARGS__)

#define LOG_FATALF(logger, fmt, ...) \
  LOG_AT_LEVELF(logger, asynclog::LogLevel::FATAL, fmt, __VA_ARGS__)
