#ifndef _LOG_LEVEL_H_
#define _LOG_LEVEL_H_

#include <string>

namespace asynclog {

// Log level.
class LogLevel {
 public:
  enum Level {
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5,
  };

  static std::string toString(Level level) {
    switch (level) {
      case DEBUG:
        return "DEBUG";
        break;
      case INFO:
        return "INFO";
        break;
      case WARN:
        return "WARN";
        break;
      case ERROR:
        return "ERROR";
        break;
      case FATAL:
        return "FATAL";
      default:
        return "UNKNOWN";
        break;
    }
  }
};

}  // namespace asynclog

#endif  // _LOG_LEVEL_H_