#include "file_appender.h"

#include <memory>

#include "log_event.h"
#include "log_formatter.h"
#include "util/current.h"
#include "util/lockfree_queue.h"

namespace asynclog {

FileAppender::FileAppender(const std::string& file_name)
    : file_name_(file_name) {
  reopen();
}

FileAppender::~FileAppender() {}

void FileAppender::initBuffer() {
  if (!buffer_) {
    buffer_ = std::make_unique<lockfreebuf::LockFreeQueue<LogEventPtr>>();
    // buffer_ = BufferPtr(new lockfreebuf::LockFreeQueue<LogEventPtr>());
    // TODO: handle init fail case.
    buffer_->initialize();
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
      // TODO: handle error.
    }
  }
}

void FileAppender::produce(LogLevel::Level level, LogEventPtr event) {
  // TODO: handle init fail case
  if (!buffer_ || !buffer_->initialize()) {
    auto init_fail_event = std::make_shared<asynclog::LogEvent>(
        event->getLogger(), LogLevel::Level::ERROR, __FILE__, __LINE__, 0,
        current::tid(), 2, time(0), "test_thread");
    init_fail_event->getContentSS()
        << "file appender buffer dose not exist, use sync log";
    appendLog(LogLevel::Level::ERROR, init_fail_event);
    appendLog(level, event);
    return;
  }

  if (level >= limit_level_) {
    auto status = buffer_->Enqueue(event);
    if (!status) {
      appendLog(level, event);
    }
  }
}

void FileAppender::consume() {
  LogEventPtr ptr;
  // TODO: consider multi collector.
  while (buffer_->Dequeue(ptr)) {
    uint64_t now = ptr->getTime();
    if (now >= (lasttime_ + 3)) {
      reopen();
      lasttime_ = now;
    }

    if (!log_formatter_->format(file_stream_, ptr->getLevel(), ptr)) {
      // TODO: handle error.
    }
  }
}

}  // namespace asynclog
