#include "log_collector.h"

#include <chrono>
#include <functional>
#include <thread>

#include "logger.h"

namespace asynclog {

LogCollector::LogCollector(const LoggerPtr logger, const InitCallback &cb,
                           const std::string &name)
    : exiting_(false),
      thread_(std::bind(&LogCollector::collect, this), name),
      init_callback_(cb),
      logger_(logger) {}

LogCollector::~LogCollector() {}

void LogCollector::stop() {
  exiting_ = true;
  thread_.join();
  logger_.reset();
}

void LogCollector::collect() {
  while (1) {
    for (auto &appender : logger_->getAppenders()) {
      appender->consume();
    }
    if (exiting_) {
      cleanup();
      break;
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }
}

void LogCollector::cleanup() {
  for (auto &appender : logger_->getAppenders()) {
    appender->consume();
  }
}

void LogCollector::run() { thread_.start(); }

}  // namespace asynclog
