#include "stdout_appender.h"

#include <iostream>
#include <memory>

#include "log_formatter.h"

namespace asynclog {

void StdoutAppender::appendLog(LogLevel::Level level, LogEventPtr event) {
  if (level >= limit_level_) {
    log_formatter_->format(std::cout, level, event);
  }
}

void StdoutAppender::produce(LogLevel::Level level, LogEventPtr event) {
  if (level >= limit_level_) {
    log_formatter_->format(std::cout, level, event);
  }
}

void StdoutAppender::consume() {}

void StdoutAppender::asyncInit() {}

}  // namespace asynclog
