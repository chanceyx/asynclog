#pragma once

#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "log_appender.h"
#include "log_level.h"
#include "util/noncopyable.h"

namespace asynclog {

class LogEvent;
class Logger;

// FileAppender is used to append log to a file.
class FileAppender : noncopyable, public LogAppender {
 public:
  using LoggerPtr = std::shared_ptr<Logger>;
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using FileLogAppenderPtr = std::shared_ptr<FileAppender>;

  FileAppender(const std::string& file_name);

  // Append log to a file.
  void appendLog(LogLevel::Level level, LogEventPtr event) override;

  // Reopen the target file.
  bool reopen();

 private:
  // File name.
  std::string file_name_;

  // File stream.
  std::ofstream file_stream_;

  // File's last open time.
  uint64_t lasttime_ = 0;
};

}  // namespace asynclog