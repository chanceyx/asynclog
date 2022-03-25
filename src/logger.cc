#include "logger.h"

#include <iostream>
#include <string>

#include "file_appender.h"
#include "log_appender.h"
#include "log_formatter.h"
#include "stdout_appender.h"
#include "util/singleton.h"

namespace asynclog {

Logger::LoggerPtr Logger::DefaultLogger() {
  static auto default_logger =
      std::shared_ptr<Logger>(new Logger("default_logger"));
  default_logger->addAppender(
      std::shared_ptr<LogAppender>(new FileAppender("default.log")));
  return default_logger;
}

Logger::Logger(const std::string &name) : name_(name) {
  default_formatter_.reset(
      new LogFormatter("%d{%Y-%m-%d %H:%M:%S} %t %N %F [%c] %f:%l [%p] %m%n"));
}

Logger::LoggerPtr Logger::makeLogger(const std::string &name) {
  auto logger = std::shared_ptr<Logger>(new Logger(name));
  return logger;
}

Logger::LogAppenderPtr Logger::makeFileAppender(const std::string &file_name) {
  auto file_appender =
      std::shared_ptr<LogAppender>(new FileAppender(file_name));
  return file_appender;
}

Logger::LogAppenderPtr Logger::makeStdoutAppender() {
  auto stdout_appender = std::shared_ptr<LogAppender>(new StdoutAppender);
  return stdout_appender;
}

Logger::LogFormatterPtr Logger::makeFormatter(const std::string &pattern) {
  auto log_formatter = std::shared_ptr<LogFormatter>(new LogFormatter(pattern));
  return log_formatter;
}

Logger::LogEventPtr Logger::makeEvent(LogLevel::Level level) {
  auto log_event = std::shared_ptr<LogEvent>(
      new LogEvent(shared_from_this(), level, __FILE__, __LINE__, 0, 1, 1,
                   time(0), "test_thread"));
  return log_event;
}

void Logger::addAppender(LogAppenderPtr appender) {
  // appender's logformatter is null, give it a default formatter.
  if (!appender->getFormatter()) {
    appender->setFormatter(default_formatter_);
  }
  appenders_.push_back(appender);
}

void Logger::removeAppender(LogAppenderPtr appender) {
  for (auto it = appenders_.begin(); it != appenders_.end(); it++) {
    if (*it == appender) {
      appenders_.erase(it);
      break;
    }
  }
}

void Logger::log(LogLevel::Level level, LogEventPtr event) {
  if (level >= limit_level_) {
    for (auto &appender : appenders_) {
      appender->appendLog(level, event);
    }
  }
}

void Logger::debug(LogEventPtr event) { log(LogLevel::DEBUG, event); }

void Logger::info(LogEventPtr event) { log(LogLevel::INFO, event); }

void Logger::fatal(LogEventPtr event) { log(LogLevel::FATAL, event); }

void Logger::error(LogEventPtr event) { log(LogLevel::ERROR, event); }

}  // namespace asynclog