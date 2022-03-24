#include "file_appender.h"

#include <iostream>

#include "log_event.h"
#include "log_formatter.h"

namespace asynclog {

FileLogAppender::FileLogAppender(const std::string& file_name)
    : file_name_(file_name) {
  reopen();
}

bool FileLogAppender::reopen() {
  if (file_stream_) {
    file_stream_.close();
  }
  file_stream_.open(file_name_, std::ios_base::app);
  return !!file_stream_;
}

void FileLogAppender::appendLog(LoggerPtr logger, LogLevel::Level level,
                                LogEventPtr event) {
  if (level >= level_) {
    uint64_t now = event->getTime();
    if (now >= (lasttime_ + 1)) {
      reopen();
      lasttime_ = now;
    }

    if (!log_formatter_->format(file_stream_, logger, level, event)) {
      std::cout << "error" << std::endl;
    }
  }
}
}  // namespace armsy
