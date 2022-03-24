#include "logger.h"

#include <iostream>
#include <string>

#include "file_appender.h"
#include "log_appender.h"
#include "log_formatter.h"
#include "stdout_appender.h"

namespace asynclog {

Logger::LoggerPtr Logger::DefaultLogger() {
  static auto logger = std::shared_ptr<Logger>(new Logger("default_logger"));
  logger->addAppender(
      std::shared_ptr<LogAppender>(new FileLogAppender("default_log.log")));
  logger->addAppender(std::shared_ptr<LogAppender>(new StdoutAppender()));
  return logger;
}

Logger::Logger(const std::string &name) : name_(name) {
  default_formatter_.reset(new LogFormatter(
      "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
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
    auto self = shared_from_this();
    for (auto &appender : appenders_) {
      appender->appendLog(self, level, event);
    }
  }
}

void Logger::debug(LogEventPtr event) { log(LogLevel::DEBUG, event); }

void Logger::info(LogEventPtr event) { log(LogLevel::INFO, event); }

void Logger::fatal(LogEventPtr event) { log(LogLevel::FATAL, event); }

void Logger::error(LogEventPtr event) { log(LogLevel::ERROR, event); }

}  // namespace armsy