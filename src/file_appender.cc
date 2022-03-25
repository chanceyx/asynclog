#include "file_appender.h"

#include <iostream>

#include "log_event.h"
#include "log_formatter.h"

namespace asynclog {

FileAppender::FileAppender(const std::string& file_name)
    : file_name_(file_name) {
  reopen();
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
    if (now >= (lasttime_ + 1)) {
      reopen();
      lasttime_ = now;
    }

    if (!log_formatter_->format(file_stream_, level, event)) {
      std::cout << "error" << std::endl;
    }
  }
}

}  // namespace asynclog
