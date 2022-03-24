#pragma once

#include "log_appender.h"
#include "log_level.h"

namespace asynclog {

class LogEvent;

class StdoutAppender : public LogAppender {
 public:
  void appendLog(LoggerPtr logger, LogLevel::Level level, LogEventPtr event) override;
};
}  // namespace armsy
