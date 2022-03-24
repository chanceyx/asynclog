#pragma once

#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "log_appender.h"
#include "log_level.h"

namespace asynclog {

class LogEvent;
class Logger;

class FileLogAppender : public LogAppender {
 public:
  using LoggerPtr = std::shared_ptr<Logger>;
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using FileLogAppenderPtr = std::shared_ptr<FileLogAppender>;

  FileLogAppender(const std::string& file_name);
  void appendLog(LoggerPtr logger, LogLevel::Level level,
                 LogEventPtr event) override;
  bool reopen();

 private:
  std::string file_name_;
  std::ofstream file_stream_;
  uint64_t lasttime_;
};
}  // namespace armsy