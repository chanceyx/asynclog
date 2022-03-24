#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "log_level.h"

namespace asynclog {

class LogEvent;
class Logger;
class LogEvent;
class FormatItem;

// LogFormatter defines a log format for logging.
// A legal pattern: "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
// %m log output
// %p log level
// %r last-time(ms)
// %c log name
// %t thread id
// %n end of line
// %d time of the date
// %f file's name
// %l line number
// %T tab
// %F fiber id
// %N thread name
class LogFormatter {
 public:
  using FormatItemPtr = std::shared_ptr<FormatItem>;
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using LoggerPtr = std::shared_ptr<Logger>;

  LogFormatter(const std::string &pattern);

  // Format the event to string base on pattern_.
  std::string format(LoggerPtr logger, LogLevel::Level level,
                     LogEventPtr event);

  // Format the event to ostream base on pattern_.
  std::ostream &format(std::ostream &ofs, LoggerPtr logger,
                       LogLevel::Level level, LogEventPtr event);

  // Parse the pattern_, this function is called by the constructor. If the
  // pattern_ is illegal, error_ is set to true.
  void parse();

  // If the pattern_ is legal.
  bool isError() const { return error_; }

  // Return the pattern_.
  const std::string getPattern() const { return pattern_; }

 private:
  // Format pattern.
  std::string pattern_;

  // Pattern's options.
  std::vector<FormatItemPtr> items_;

  // If error occurs when parse pattern_.
  bool error_;
};
}  // namespace asynclog
