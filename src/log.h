#pragma once

#include "log_event.h"

#define LOG_AT_LEVEL(logger, level)                                            \
  if (logger->getLevel() <= level)                                             \
  asynclog::LogEventWrapper(std::shared_ptr<asynclog::LogEvent>(new asynclog::LogEvent( \
                             logger, level, __FILE__, __LINE__, 0, 1, 2,       \
                             time(0), "test_thread")))                         \
      .getEventStirngStream()

#define LOG_DEBUG(logger) LOG_AT_LEVEL(logger, armsy::LogLevel::DEBUG)

#define LOG_INFO(logger) LOG_AT_LEVEL(logger, armsy::LogLevel::INFO)

#define LOG_WARN(logger) LOG_AT_LEVEL(logger, armsy::LogLevel::WARN)

#define LOG_ERROR(logger) LOG_AT_LEVEL(logger, armsy::LogLevel::ERROR)

#define LOG_FATAL(logger) LOG_AT_LEVEL(logger, armsy::LogLevel::FATAL)