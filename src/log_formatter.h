#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "log_level.h"

namespace asynclog {

class LogEvent;
class Logger;
class LogEvent;
class FormatItem;

/**
 * @brief 构造函数
 * @param[in] pattern 格式模板
 * @details
 *  %m 消息
 *  %p 日志级别
 *  %r 累计毫秒数
 *  %c 日志名称
 *  %t 线程id
 *  %n 换行
 *  %d 时间
 *  %f 文件名
 *  %l 行号
 *  %T 制表符
 *  %F 协程id
 *  %N 线程名称
 *
 *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
 */

class LogFormatter {
 public:
  using FormatItemPtr = std::shared_ptr<FormatItem>;
  using LogEventPtr = std::shared_ptr<LogEvent>;
  using LoggerPtr = std::shared_ptr<Logger>;

  LogFormatter(const std::string &pattern);

  // Format the event to string base on pattern_.
  std::string format(LoggerPtr logger, LogLevel::Level level,
                     LogEventPtr event);

  // Format the event to ostream base on pattern_.
  std::ostream &format(std::ostream &ofs, LoggerPtr logger,
                       LogLevel::Level level, LogEventPtr event);

  // Parse the pattern_, this function is called by the constructor. If the
  // pattern_ is illegal, error_ is set to true.
  void parse();

  // If the pattern_ is legal.
  bool isError() const { return error_; }

  // Return the pattern_.
  const std::string getPattern() const { return pattern_; }

 private:
  // Format pattern.
  std::string pattern_;

  // Pattern's options.
  std::vector<FormatItemPtr> items_;

  // If error occurs when parse pattern_.
  bool error_;
};
}  // namespace armsy
