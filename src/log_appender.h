#pragma once

#include <memory>
#include <string>

#include "log_level.h"

namespace asynclog {

class LogFormatter;
class LogEvent;
class Logger;

class LogAppender {
 public:
  using LogFormatterPtr = std::shared_ptr<LogFormatter>;
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using LoggerPtr = std::shared_ptr<Logger>;

  virtual ~LogAppender() {}
  virtual void appendLog(LoggerPtr logger, LogLevel::Level level,
                   LogEventPtr event) = 0;
  void setFormatter(LogFormatterPtr log_formatter) {
    log_formatter_ = log_formatter;
  }
  LogFormatterPtr getFormatter() const { return log_formatter_; }

 protected:
  LogLevel::Level level_ = LogLevel::DEBUG;
  LogFormatterPtr log_formatter_;
};
}  // namespace armsy
