#pragma once

#include <stdarg.h>

#include <memory>
#include <sstream>
#include <string>

#include "log_level.h"
#include "util/singleton.h"

namespace asynclog {

class LoggerRaw;

// LogEvent is a event need to be record in log.
class LogEvent : noncopyable {
 public:
  using LoggerRawPtr = std::shared_ptr<LoggerRaw>;
  LogEvent(LoggerRawPtr logger, LogLevel::Level level, const char* file,
           int32_t line, uint32_t elapse, uint32_t thread_id, uint32_t fiber_id,
           uint64_t time, const std::string& thread_name);

  ~LogEvent();

  std::string getContent() const { return content_ss_.str(); }
  std::string getThreadName() const { return t_name_; }
  LoggerRawPtr getLogger() const { return logger_; }
  LogLevel::Level getLevel() const { return level_; }
  std::stringstream& getContentSS() { return content_ss_; }
  const char* getFileName() const { return file_; }
  int32_t getLine() const { return line_; }
  uint32_t getElapse() const { return elapse_; }
  uint32_t getTid() const { return tid_; }
  uint32_t getFid() const { return fid_; }
  uint64_t getTime() const { return time_; }

  // Build content with c style format.
  void format(const char* fmt, ...);

 private:
  // Build content with c style format.
  void format(const char* fmt, va_list al);

  // Name of the file to output.
  const char* file_;

  // Line in the file_.
  int32_t line_;

  // Machine time.
  uint32_t elapse_;

  // Thread id of the logging thread.
  uint32_t tid_;

  // Thread name of the logging thread.
  std::string t_name_;

  // Fiber id of the logging fiber.
  uint32_t fid_;

  // Epoch Time.
  uint64_t time_;

  // Stringstream of the log content.
  std::stringstream content_ss_;

  // Logger of this log event.
  LoggerRawPtr logger_;

  LogLevel::Level level_;
};

// LogEventWrapper wraps a log event used to log as stream.
class LogEventWrapper {
 public:
  using LogEventPtr = std::shared_ptr<LogEvent>;
  LogEventWrapper(LogEventPtr event);

  // A log event will be append to log file when a LogEventWrapper calls
  // destructor.
  ~LogEventWrapper();

  // Get sstream.
  std::stringstream& getEventStirngStream();

  // Get event_.
  LogEventPtr getEvent() const { return event_; }

 private:
  // A log event.
  LogEventPtr event_;
};

}  // namespace asynclog