#pragma once

#include <stdint.h>

#include <fstream>
#include <list>
#include <memory>
#include <sstream>
#include <string>

#include "log.h"
#include "log_appender.h"
#include "log_level.h"

namespace asynclog {

class LogFormatter;
class LogEvent;
class LogAppender;

// Logger is class use for logging, when you get a object logger, you can use
// it to log like this: logger.log(LogEvent::INFO, event);
// logger.debug(event);
class Logger : public std::enable_shared_from_this<Logger> {
 public:
  using LoggerPtr = std::shared_ptr<Logger>;
  using LogFormatterPtr = std::shared_ptr<LogFormatter>;
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using LogAppenderPtr = std::shared_ptr<LogAppender>;

  Logger(const std::string &name = "default_logger");

  // Return logger's name_.
  std::string getName() const { return name_; }

  // Log the event.
  void log(LogLevel::Level level, LogEventPtr event);

  // Log the event by given log level.
  void debug(LogEventPtr event);
  void info(LogEventPtr event);
  void warn(LogEventPtr event);
  void error(LogEventPtr event);
  void fatal(LogEventPtr event);

  // Add a log appender for logger.
  void addAppender(LogAppenderPtr appender);

  // Remove a log appender for logger.
  void removeAppender(LogAppenderPtr appender);

  // Return a new log event.
  LogEventPtr makeEvent(LogLevel::Level level);

  // Get the limit log level of the logger.
  LogLevel::Level getLevel() const { return limit_level_; }

  // Set the limit log level of the logger.
  void setLevel(LogLevel::Level level) { limit_level_ = level; }

 private:
  // Logger's name.
  std::string name_;

  // Logger's limit level, only the event which's level >= limit_level_ will
  // be log by logger.
  LogLevel::Level limit_level_;

  // Logger's appender.
  std::list<LogAppenderPtr> appenders_;

  // Logger's default log formatter, if a log appender's formatter is not set
  // in function addAppender() the appender's formatter will be set to
  // default_formatter_.
  LogFormatterPtr default_formatter_;
};

// Get the default logger.
std::shared_ptr<Logger> DefaultLogger();

// Return a new logger.
std::shared_ptr<Logger> makeLogger(const std::string &name);

// Return a new stdout appender.
std::shared_ptr<LogAppender> makeStdoutAppender();

// Return a new file appender.
std::shared_ptr<LogAppender> makeFileAppender(const std::string &file_name);

// Return a new log formatter.
std::shared_ptr<LogFormatter> makeFormatter(const std::string &pattern);

}  // namespace asynclog