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
class Logger;

class LogCollector : noncopyable {
 public:
  using InitCallback = std::function<void()>;
  using LogAppenderPtr = std::shared_ptr<LogAppender>;
  using LoggerPtr = std::shared_ptr<Logger>;

  LogCollector(const LoggerPtr logger, const InitCallback &cb = InitCallback(),
               const std::string &name = std::string());

  ~LogCollector();

  void run();
  void stop();

 private:
  // Collect log events in buffer, and store them in file.
  void collect();

  void cleanup();

  // Is collector is exiting.
  bool exiting_;

  // Collector's thread.
  Thread thread_;

  // Initialize callback.
  InitCallback init_callback_;

  LoggerPtr logger_;
};

}  // namespace asynclog
