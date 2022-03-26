#pragma once

#include "log_event.h"
#include "util/current_thread.h"

// Stream style log.
// Usage:
//  LOG_AT_LEVEL(logger, asynclog::LogLevel::FATAL) << "fatal info";
//  LOG_WARN(logger) << "error info";
#define LOG(logger, level)                                               \
  if (logger->getLevel() <= level)                                       \
  asynclog::LogEventWrapper(                                             \
      std::shared_ptr<asynclog::LogEvent>(new asynclog::LogEvent(        \
          logger, level, __FILE__, __LINE__, 0, CurrentThread::tid(), 2, \
          time(0), "test_thread")))                                      \
      .getEventStirngStream()

#define LOG_DEBUG(logger) LOG(logger, asynclog::LogLevel::DEBUG)

#define LOG_INFO(logger) LOG(logger, asynclog::LogLevel::INFO)

#define LOG_WARN(logger) LOG(logger, asynclog::LogLevel::WARN)

#define LOG_ERROR(logger) LOG(logger, asynclog::LogLevel::ERROR)

#define LOG_FATAL(logger) LOG(logger, asynclog::LogLevel::FATAL)

// Format style log.
// Usage:
//  LOG_F(logger, asynclog::LogLevel::FATAL, "%s - errno: %d",
//               "fatal info", errno);
//  LOG_ERROR_F(logger, "%s - errno: %d", "error info", errno);
#define LOG_F(logger, level, fmt, ...)                                   \
  if (logger->getLevel() <= level)                                       \
  asynclog::LogEventWrapper(                                             \
      std::shared_ptr<asynclog::LogEvent>(new asynclog::LogEvent(        \
          logger, level, __FILE__, __LINE__, 0, CurrentThread::tid(), 2, \
          time(0), "test_thread")))                                      \
      .getEvent()                                                        \
      ->format(fmt, __VA_ARGS__)

#define LOG_DEBUG_F(logger, fmt, ...) \
  LOG_F(logger, asynclog::LogLevel::DEBUG, fmt, __VA_ARGS__)

#define LOG_INFO_F(logger, fmt, ...) \
  LOG_F(logger, asynclog::LogLevel::INFO, fmt, __VA_ARGS__)

#define LOG_WARN_F(logger, fmt, ...) \
  LOG_F(logger, asynclog::LogLevel::WARN, fmt, __VA_ARGS__)

#define LOG_ERROR_F(logger, fmt, ...) \
  LOG_F(logger, asynclog::LogLevel::ERROR, fmt, __VA_ARGS__)

#define LOG_FATAL_F(logger, fmt, ...) \
  LOG_F(logger, asynclog::LogLevel::FATAL, fmt, __VA_ARGS__)
