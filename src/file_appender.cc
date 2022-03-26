#include "file_appender.h"

#include <iostream>
#include <memory>

#include "log_event.h"
#include "log_formatter.h"
#include "util/lockfree_queue.h"

namespace asynclog {

FileAppender::FileAppender(const std::string& file_name, bool async_logging)
    : file_name_(file_name) {
  reopen();
  if (async_logging) {
    current_buffer_ = BufferPtr(new lockfreebuf::LockFreeQueue<LogEventPtr>);
    if (!current_buffer_->initialize()) {
      std::cout << "buffer init failed" << std::endl;
    }
  }
}

bool FileAppender::reopen() {
  if (file_stream_) {
    file_stream_.close();
  }
  file_stream_.open(file_name_, std::ios_base::app);
  return !!file_stream_;
}

void FileAppender::appendLog(LogLevel::Level level, LogEventPtr event) {
  if (level >= limit_level_) {
    uint64_t now = event->getTime();
    if (now >= (lasttime_ + 3)) {
      reopen();
      lasttime_ = now;
    }

    if (!log_formatter_->format(file_stream_, level, event)) {
      std::cout << "error" << std::endl;
    }
  }
}

void FileAppender::pushLog(LogLevel::Level level, LogEventPtr event) {
  if (current_buffer_ == nullptr) {
    std::cout << "buffer dose not exist, use sync log" << std::endl;
    appendLog(level, event);
    return;
  }

  if (level >= limit_level_) {
    int cnt = 0;
    while (cnt < kRetry_ && !current_buffer_->Enqueue(event)) cnt++;
  }
}

void FileAppender::consume() {
  LogEventPtr ptr;
  while (current_buffer_->Dequeue(ptr)) {
  }
}

}  // namespace asynclog
