#include "logger.h"

#include <string>
#include <thread>

#include "file_appender.h"
#include "log_appender.h"
#include "log_collector.h"
#include "log_formatter.h"
#include "stdout_appender.h"

namespace asynclog {

std::shared_ptr<LoggerRaw> DefaultLoggerRaw() {
  static auto default_logger =
      std::make_shared<LoggerRaw>("default_logger", DISABLE);
  default_logger->addAppender(
      std::shared_ptr<LogAppender>(new FileAppender("default.log")));
  return default_logger;
}

LoggerRaw::LoggerRaw(const std::string &name, ASYNC async) : name_(name) {
  async_enabled_ = (async == ENABLE ? true : false);
  default_formatter_.reset(
      new LogFormatter("%d{%Y-%m-%d %H:%M:%S} %t %N [%c] %f:%l [%p] %m%n"));
}

void LoggerRaw::setDefautlFormat(const std::string &format) {
  default_formatter_.reset(new LogFormatter(format));
}

LoggerRaw::~LoggerRaw() {}

void LoggerRaw::init() {
  if (!collector_) {
    collector_ = std::make_shared<LogCollector>(shared_from_this());
    collector_->run();
  }
}

void LoggerRaw::drop() {
  if (collector_) collector_->stop();
}

LoggerRaw::LogEventPtr LoggerRaw::makeEvent(LogLevel::Level level) {
  auto log_event =
      std::make_shared<LogEvent>(shared_from_this(), level, __FILE__, __LINE__,
                                 0, 1, 1, time(0), "test_thread");
  return log_event;
}

void LoggerRaw::addAppender(LogAppenderPtr appender) {
  // appender's logformatter is null, give it a default formatter.
  if (!appender->getFormatter()) {
    appender->setFormatter(default_formatter_);
  }
  if (async_enabled_) appender->initBuffer();
  appenders_.push_back(appender);
}

void LoggerRaw::removeAppender(LogAppenderPtr appender) {
  for (auto it = appenders_.begin(); it != appenders_.end(); it++) {
    if (*it == appender) {
      appenders_.erase(it);
      break;
    }
  }
}

void LoggerRaw::log(LogLevel::Level level, LogEventPtr event) {
  if (async_enabled_) {
    asynclog(level, event);
  } else {
    synclog(level, event);
  }
}

void LoggerRaw::synclog(LogLevel::Level level, LogEventPtr event) {
  if (level >= limit_level_) {
    for (auto &appender : appenders_) {
      appender->appendLog(level, event);
    }
  }
}

void LoggerRaw::asynclog(LogLevel::Level level, LogEventPtr event) {
  if (level >= limit_level_) {
    for (auto &appender : appenders_) {
      appender->produce(level, event);
    }
  }
}

void LoggerRaw::debug(LogEventPtr event) { log(LogLevel::DEBUG, event); }

void LoggerRaw::info(LogEventPtr event) { log(LogLevel::INFO, event); }

void LoggerRaw::fatal(LogEventPtr event) { log(LogLevel::FATAL, event); }

void LoggerRaw::error(LogEventPtr event) { log(LogLevel::ERROR, event); }

Logger::Logger(std::shared_ptr<LoggerRaw> logger) : logger_(logger) {
  if (!logger_) {
    logger_ = DefaultLoggerRaw();
  }
}

Logger::Logger(const std::string &name, ASYNC async)
    : logger_(makeLoggerRaw(name, async)) {
  if (logger_->isAsync()) logger_->init();
}

Logger::~Logger() { logger_->drop(); }

void Logger::addAppender(LogAppenderPtr appender) {
  logger_->addAppender(appender);
}
void Logger::removeAppender(LogAppenderPtr appender) {
  logger_->removeAppender(appender);
}

Logger::LogEventPtr Logger::makeEvent(LogLevel::Level level) {
  return logger_->makeEvent(level);
}

void Logger::setDefaultFormat(const std::string &format) {
  logger_->setDefautlFormat(format);
}

std::shared_ptr<LoggerRaw> makeLoggerRaw(const std::string &name, ASYNC async) {
  auto logger = std::make_shared<LoggerRaw>(name, async);
  return logger;
}

std::shared_ptr<LogAppender> makeFileAppender(const std::string &file_name) {
  std::shared_ptr<LogAppender> file_appender =
      std::make_shared<FileAppender>(file_name);
  return file_appender;
}

std::shared_ptr<LogAppender> makeStdoutAppender() {
  std::shared_ptr<LogAppender> stdout_appender =
      std::make_shared<StdoutAppender>();
  return stdout_appender;
}

std::shared_ptr<LogFormatter> makeFormatter(const std::string &pattern) {
  auto log_formatter = std::make_shared<LogFormatter>(pattern);
  return log_formatter;
}

}  // namespace asynclog