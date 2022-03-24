#include "stdout_appender.h"

#include <iostream>
#include <memory>

#include "log_formatter.h"

namespace asynclog {

void StdoutAppender::appendLog(LoggerPtr logger, LogLevel::Level level,
                         LogEventPtr event) {
  if (level >= level_) {
    log_formatter_->format(std::cout, logger, level, event);
  }
}

}  // namespace armsy
