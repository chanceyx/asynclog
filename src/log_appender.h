#pragma once

#include <memory>
#include <string>

#include "log_level.h"

namespace asynclog {

class LogFormatter;
class LogEvent;
class Logger;

// LogAppender is used to append log to a specific place.
class LogAppender {
 public:
  using LogFormatterPtr = std::shared_ptr<LogFormatter>;
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using LoggerPtr = std::shared_ptr<Logger>;

  virtual ~LogAppender() {}

  // Append log to a specific place.
  virtual void appendLog(LoggerPtr logger, LogLevel::Level level,
                         LogEventPtr event) = 0;

  // Set a formatter of the appender.
  void setFormatter(LogFormatterPtr log_formatter) {
    log_formatter_ = log_formatter;
  }

  // Get a formatter of the appender.
  LogFormatterPtr getFormatter() const { return log_formatter_; }

 protected:
  //
  LogLevel::Level limit_level_ = LogLevel::DEBUG;
  LogFormatterPtr log_formatter_;
};

}  // namespace asynclog
