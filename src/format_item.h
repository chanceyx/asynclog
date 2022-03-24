#pragma once

#include <time.h>

#include <memory>
#include <string>

#include "log_event.h"
#include "log_level.h"
#include "logger.h"

namespace asynclog {

class Logger;

class FormatItem {
 public:
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using LoggerPtr = std::shared_ptr<Logger>;
  using FormatItemPtr = std::shared_ptr<FormatItem>;

  virtual ~FormatItem() {}
  virtual void format(std::ostream &os, std::shared_ptr<Logger> logger,
                      LogLevel::Level level, LogEventPtr event) = 0;
};

class MsgItem : public FormatItem {
 public:
  MsgItem(const std::string &str = "") {}

  void format(std::ostream &os, LoggerPtr logger, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getContent();
  }
};

class LevelItem : public FormatItem {
 public:
  LevelItem(const std::string &str = "") {}

  void format(std::ostream &os, LoggerPtr logger, LogLevel::Level level,
              LogEventPtr event) override {
    os << LogLevel::toString(level);
  }
};

class ElapseItem : public FormatItem {
 public:
  ElapseItem(const std::string &str = "") {}

  void format(std::ostream &os, LoggerPtr logger, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getElapse();
  }
};

class NameItem : public FormatItem {
 public:
  NameItem(const std::string &str = "") {}
  void format(std::ostream &os, LoggerPtr logger, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getLogger()->getName();
  }
};

class ThreadIdItem : public FormatItem {
 public:
  ThreadIdItem(const std::string &str = "") {}
  void format(std::ostream &os, LoggerPtr logger, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getTid();
  }
};

class ThreadNameItem : public FormatItem {
 public:
  ThreadNameItem(const std::string &str = "") {}
  void format(std::ostream &os, LoggerPtr logger, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getThreadName();
  }
};

class FiberItem : public FormatItem {
 public:
  FiberItem(const std::string &str = "") {}
  void format(std::ostream &os, LoggerPtr logger, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getFid();
  }
};

class DateTimeItem : public FormatItem {
 public:
  DateTimeItem(const std::string &format = "%Y-%m-%d %H:%M:%S")
      : date_time_format_(format) {
    if (date_time_format_.empty()) {
      date_time_format_ = "%Y-%m-%d %H:%M:%S";
    }
  }

  void format(std::ostream &os, LoggerPtr logger, LogLevel::Level level,
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

class FileNameItem : public FormatItem {
 public:
  FileNameItem(const std::string &str = "") {}

  void format(std::ostream &os, LoggerPtr logger, LogLevel::Level level,
              LogEventPtr event) override {
    os << event->getFileName();
  }
};

class NewLineItem : public FormatItem {
 public:
  NewLineItem(const std::string &str = "") {}

  void format(std::ostream &os, LoggerPtr logger, LogLevel::Level level,
              LogEventPtr event) override {
    os << std::endl;
  }
};

class StringItem : public FormatItem {
 public:
  StringItem(const std::string &str) : str_(str) {}

  void format(std::ostream &os, LoggerPtr logger, LogLevel::Level level,
              LogEventPtr event) override {
    os << str_;
  }

 private:
  std::string str_;
};

class TabItem : public FormatItem {
 public:
  TabItem(const std::string &str = "") {}

  void format(std::ostream &os, LoggerPtr logger, LogLevel::Level level,
              LogEventPtr event) override {
    os << "\t";
  }
};

}  // namespace armsy
