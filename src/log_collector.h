#pragma once

#include <condition_variable>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <string>

#include "util/singleton.h"
#include "util/thread.h"

namespace asynclog {

class LogAppender;
class LoggerRaw;

class LogCollector : noncopyable {
 public:
  using InitCallback = std::function<void()>;
  using LogAppenderPtr = std::shared_ptr<LogAppender>;
  using LoggerRawPtr = std::shared_ptr<LoggerRaw>;

  LogCollector(const LoggerRawPtr logger,
               const InitCallback &cb = InitCallback(),
               const std::string &name = std::string());

  ~LogCollector();

  // Start a log collect thread.
  void run();

  // Stop and join the log collect thread.
  void stop();

 private:
  // Collect log events in buffer, and store them in file.
  void collect();

  // Clean up the log buffer_.
  void cleanup();

  // Is collector is exiting.
  bool exiting_;

  // Collector's thread.
  Thread thread_;

  // Initialize callback.
  InitCallback init_callback_;

  // Logger.
  LoggerRawPtr logger_;
};

}  // namespace asynclog
