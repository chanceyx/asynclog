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

}  // namespace asynclog
