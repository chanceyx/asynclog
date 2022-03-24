#pragma once

#include "log_appender.h"
#include "log_level.h"

namespace asynclog {

class LogEvent;

// StdoutAppender is a log appender use to append log to std::out.
class StdoutAppender : public LogAppender {
 public:
  void appendLog(LoggerPtr logger, LogLevel::Level level,
                 LogEventPtr event) override;
};

}  // namespace asynclog
