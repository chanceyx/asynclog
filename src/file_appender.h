#pragma once

#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "log_appender.h"
#include "log_level.h"
#include "util/lockfree_queue.h"
#include "util/singleton.h"

namespace asynclog {

class LogEvent;
class LoggerRaw;
class LockFreeQueue;

// FileAppender is used to append log to a file.
class FileAppender : noncopyable, public LogAppender {
 public:
  using LoggerRawPtr = std::shared_ptr<LoggerRaw>;
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using FileLogAppenderPtr = std::shared_ptr<FileAppender>;
  using BufferPtr = std::unique_ptr<lockfreebuf::LockFreeQueue<LogEventPtr>>;

  FileAppender(const std::string& file_name);

  ~FileAppender();

  // Append log to a file.
  void appendLog(LogLevel::Level level, LogEventPtr event) override;

  // Initialize the buffer_, it will be called when the logger uses async mod.
  void initBuffer() override;

  // Push log event into buffer_.
  void produce(LogLevel::Level level, LogEventPtr event) override;

  // Consume log event out of buffer_ and write into disk.
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

  // Log events's buffer.
  BufferPtr buffer_;
};

}  // namespace asynclog