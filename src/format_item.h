#pragma once

#include <time.h>

#include <memory>
#include <string>

#include "log_event.h"
#include "log_level.h"
#include "logger.h"

namespace asynclog {

class Logger;

// FormatItem is a single item of the formatter's pattern
// A legal pattern:
//    "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
// Format items:
//    %m log output
//    %p log level
//    %r duration(ms)
//    %c log name
//    %t thread id
//    %n end of line
//    %d time of the date
//    %f file's name
//    %l line number
//    %T tab
//    %F fiber id
//    %N thread name
class FormatItem {
 public:
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using LoggerPtr = std::shared_ptr<Logger>;
  using FormatItemPtr = std::shared_ptr<FormatItem>;

  virtual ~FormatItem() {}

  // Each format item uses format() to output its content.
  virtual void format(std::ostream &os, LogLevel::Level level,
                      LogEventPtr event) = 0;
};

// Output a log event's message of a log record.
class MsgItem : public FormatItem {
 public:
  MsgItem(const std::string &str = "") {}

  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getContent();
  }
};

// Output Log level of a log record.
class LevelItem : public FormatItem {
 public:
  LevelItem(const std::string &str = "") {}

  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    os << LogLevel::toString(level);
  }
};

// Output duration of a log event of a log record.
class ElapseItem : public FormatItem {
 public:
  ElapseItem(const std::string &str = "") {}

  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getElapse();
  }
};

// Output log name(log-set) of a log record.
class NameItem : public FormatItem {
 public:
  NameItem(const std::string &str = "") {}
  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getLogger()->getName();
  }
};

// Output log event's thread id of a log record.
class ThreadIdItem : public FormatItem {
 public:
  ThreadIdItem(const std::string &str = "") {}
  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getTid();
  }
};

// Output log event's thread name of a log record.
class ThreadNameItem : public FormatItem {
 public:
  ThreadNameItem(const std::string &str = "") {}
  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getThreadName();
  }
};

// Output log event's fiber id of a log record.
class FiberItem : public FormatItem {
 public:
  FiberItem(const std::string &str = "") {}
  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getFid();
  }
};

// Output time info of a log record.
class DateTimeItem : public FormatItem {
 public:
  DateTimeItem(const std::string &format = "%Y-%m-%d %H:%M:%S")
      : date_time_format_(format) {
    if (date_time_format_.empty()) {
      date_time_format_ = "%Y-%m-%d %H:%M:%S";
    }
  }

  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    struct tm tm;
    time_t time = event->getTime();
    localtime_r(&time, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), date_time_format_.c_str(), &tm);
    os << buf;
  }

 private:
  std::string date_time_format_;
};

// Output a log event's file name of a log record.
class FileNameItem : public FormatItem {
 public:
  FileNameItem(const std::string &str = "") {}

  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getFileName();
  }
};

// Output a line break of a log record.
class NewLineItem : public FormatItem {
 public:
  NewLineItem(const std::string &str = "") {}

  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    os << std::endl;
  }
};

// Output line number of a log record.
class LineItem : public FormatItem {
 public:
  LineItem(const std::string &str = "") {}

  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getLine();
  }
};

// Out put a spersific string message of a log record.
class StringItem : public FormatItem {
 public:
  StringItem(const std::string &str) : str_(str) {}

  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    os << str_;
  }

 private:
  std::string str_;
};

// Output a tab of a log record.
class TabItem : public FormatItem {
 public:
  TabItem(const std::string &str = "") {}

  void format(std::ostream &os, LogLevel::Level level,
              LogEventPtr event) override {
    os << "\t";
  }
};

}  // namespace asynclog
