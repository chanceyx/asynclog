#include "log_event.h"

#include "logger.h"

namespace asynclog {

LogEvent::LogEvent(LoggerPtr logger, LogLevel::Level level, const char* file,
                   int32_t line, uint32_t elapse, uint32_t thread_id,
                   uint32_t fiber_id, uint64_t time,
                   const std::string& thread_name)
    : file_(file),
      line_(line),
      elapse_(elapse),
      tid_(thread_id),
      t_name_(thread_name),
      fid_(fiber_id),
      time_(time),
      logger_(logger),
      level_(level) {}

LogEventWrapper::LogEventWrapper(LogEventWrapper::LogEventPtr event)
    : event_(event) {}

LogEventWrapper::~LogEventWrapper() {
  event_->getLogger()->log(event_->getLevel(), event_);
}

std::stringstream& LogEventWrapper::getEventStirngStream() {
  return event_->getContentSS();
}

}  // namespace armsy
