#pragma once

#include <list>
#include <memory>
#include <string>

#include "log.h"
#include "log_level.h"

namespace asynclog {

class LogFormatter;
class LogEvent;
class LogAppender;
class LogCollector;

enum ASYNC {
  DISABLE = 0,
  ENABLE = 1,
};

// Logger is class use for logging, when you get a object logger, you can use
// it to log like this: logger.log(LogEvent::INFO, event);
// logger.debug(event);
class LoggerRaw : noncopyable, public std::enable_shared_from_this<LoggerRaw> {
 public:
  using LoggerRawPtr = std::shared_ptr<LoggerRaw>;
  using LogFormatterPtr = std::shared_ptr<LogFormatter>;
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using LogAppenderPtr = std::shared_ptr<LogAppender>;
  using LogCollectorPtr = std::shared_ptr<LogCollector>;

  LoggerRaw(const std::string &name = "default_logger", ASYNC async = DISABLE);
  ~LoggerRaw();

  // Return logger's name_.
  std::string getName() const { return name_; }

  void synclog(LogLevel::Level level, LogEventPtr event);
  void asynclog(LogLevel::Level level, LogEventPtr event);

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

  std::list<LogAppenderPtr> getAppenders() const { return appenders_; }

  // Return a new log event.
  LogEventPtr makeEvent(LogLevel::Level level);

  // Get the limit log level of the logger.
  LogLevel::Level getLevel() const { return limit_level_; }

  // Set the limit log level of the logger.
  void setLevel(LogLevel::Level level) { limit_level_ = level; }

  // Set logger's default format.
  void setDefautlFormat(const std::string &format);

  // If enable async log.
  bool isAsync() const { return async_enabled_; }

  // Init collector thread in async mod.
  void init();

  // Stop the log collector thread in async mod.
  void drop();

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

  // If this logger enabled sync.
  bool async_enabled_;

  // Log collect thread.
  LogCollectorPtr collector_;
};

// Get the default logger.
std::shared_ptr<LoggerRaw> DefaultLoggerRaw();

// Return a new logger.
std::shared_ptr<LoggerRaw> makeLoggerRaw(const std::string &name,
                                         ASYNC async = DISABLE);

// Return a new stdout appender.
std::shared_ptr<LogAppender> makeStdoutAppender();

// Return a new file appender.
std::shared_ptr<LogAppender> makeFileAppender(const std::string &file_name);

// Return a new log formatter.
std::shared_ptr<LogFormatter> makeFormatter(const std::string &pattern);

// A wrapper of LoggerRaw.
class Logger : noncopyable {
 public:
  using LoggerRawPtr = std::shared_ptr<LoggerRaw>;
  using LogFormatterPtr = std::shared_ptr<LogFormatter>;
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using LogAppenderPtr = std::shared_ptr<LogAppender>;
  using LogCollectorPtr = std::shared_ptr<LogCollector>;

  Logger(std::shared_ptr<LoggerRaw> logger = DefaultLoggerRaw());
  Logger(const std::string &name, ASYNC async = DISABLE);
  ~Logger();
  std::string getName() const { return logger_->getName(); }
  LogLevel::Level getLevel() const { return logger_->getLevel(); }
  void addAppender(LogAppenderPtr appender);
  void removeAppender(LogAppenderPtr appender);
  LogEventPtr makeEvent(LogLevel::Level level);
  void setLevel(LogLevel::Level level) { logger_->setLevel(level); }
  void setDefaultFormat(const std::string &format);
  LoggerRawPtr raw() const { return logger_; }

 private:
  std::shared_ptr<LoggerRaw> logger_;
};

}  // namespace asynclog