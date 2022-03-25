#pragma once

#include "log_appender.h"
#include "log_level.h"
#include "util/noncopyable.h"

namespace asynclog {

class LogEvent;

// StdoutAppender is a log appender use to append log to std::out.
class StdoutAppender : noncopyable, public LogAppender {
 public:
  void appendLog( LogLevel::Level level,
                 LogEventPtr event) override;
};

}  // namespace asynclog
