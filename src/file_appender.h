#pragma once

#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "log_appender.h"
#include "log_level.h"
#include "util/lockfree_queue.h"
#include "util/singleton.h"

namespace asynclog {

class LogEvent;
class Logger;
class LockFreeQueue;

// FileAppender is used to append log to a file.
class FileAppender : noncopyable, public LogAppender {
 public:
  using LoggerPtr = std::shared_ptr<Logger>;
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using FileLogAppenderPtr = std::shared_ptr<FileAppender>;
  using BufferPtr = std::unique_ptr<lockfreebuf::LockFreeQueue<LogEventPtr>>;

  FileAppender(const std::string& file_name, bool async_logging);

  // Append log to a file.
  void appendLog(LogLevel::Level level, LogEventPtr event) override;

  void pushLog(LogLevel::Level level, LogEventPtr event) override;

  void consume() override;

 private:
  // Reopen the target file.
  bool reopen();

  // Asynclogging retry time.
  const int kRetry_ = 10;

  // File name.
  std::string file_name_;

  // File stream.
  std::ofstream file_stream_;

  // File's last open time.
  uint64_t lasttime_ = 0;

  BufferPtr current_buffer_;
  BufferPtr next_buffer;
};

}  // namespace asynclog