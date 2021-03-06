#ifndef _LOG_APPENDER_
#define _LOG_APPENDER_

#include <memory>
#include <string>

#include "log_level.h"

namespace asynclog {

class LogFormatter;
class LogEvent;
class LoggerRaw;

// LogAppender is used to append log to a specific place.
class LogAppender {
 public:
  using LogFormatterPtr = std::shared_ptr<LogFormatter>;
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using LoggerRawPtr = std::shared_ptr<LoggerRaw>;

  virtual ~LogAppender() {}

  // Append log to a specific place.
  virtual void appendLog(LogLevel::Level level, LogEventPtr event) = 0;

  // Push log event into buffer_.
  virtual void produce(LogLevel::Level level, LogEventPtr event) = 0;

  // Consume log event out of buffer_ and write into disk.
  virtual void consume() = 0;

  // Initialize the buffer_, it will be called when the logger uses async mod.
  virtual void initBuffer() = 0;

  // Set a formatter of the appender.
  void setFormatter(LogFormatterPtr log_formatter) {
    log_formatter_ = log_formatter;
  }

  // Get a formatter of the appender.
  LogFormatterPtr getFormatter() const { return log_formatter_; }

 protected:
  // Limit log level of the appender.
  LogLevel::Level limit_level_ = LogLevel::DEBUG;

  // Format of the appender.
  LogFormatterPtr log_formatter_;
};

}  // namespace asynclog

#endif  // _LOG_APPENDER_
