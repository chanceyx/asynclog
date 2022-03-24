#include "log_formatter.h"

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "format_item.h"

namespace asynclog {

LogFormatter::LogFormatter(const std::string &pattern) : pattern_(pattern) {
  parse();
}

std::string LogFormatter::format(LoggerPtr logger, LogLevel::Level level,
                                 LogEventPtr event) {
  std::stringstream ss;
  for (auto &i : items_) {
    i->format(ss, logger, level, event);
  }
  return ss.str();
}
std::ostream &LogFormatter::format(std::ostream &ofs, LoggerPtr logger,
                                   LogLevel::Level level, LogEventPtr event) {
  for (auto &i : items_) {
    i->format(ofs, logger, level, event);
  }
  return ofs;
}

void LogFormatter::parse() {
  using FormatType = int;
  const FormatType kString = 0;
  const FormatType kOption = 1;

  const int kMeetOpenBrace = 1;
  const int kMeetCloseBrace = 0;

  // tuple = <str>:<sub-format>:<type>
  // type = kString or kOption
  // sub-format = the string in {},
  // e.g. parse("%d{%Y-%m-%d %H:%M:%S}")
  //      str = "d"
  //      sub-format = "%Y-%m-%d %H:%M:%S"
  //      type = kOption
  std::vector<std::tuple<std::string, std::string, int>> vec;
  std::string str;
  for (size_t i = 0; i < pattern_.size(); i++) {
    // parse string
    if (pattern_[i] != '%') {
      str.append(1, pattern_[i]);
      continue;
    }

    // parse '%%'
    if ((i + 1) < pattern_.size()) {
      if (pattern_[i + 1] == '%') {
        str.append(1, '%');
        continue;
      }
    }

    size_t n = i + 1;
    int fmt_status = 0;
    size_t sub_format_begin = 0;

    std::string option;
    std::string sub_format;

    // TODO: use stack.
    while (n < pattern_.size()) {
      if (fmt_status == kMeetCloseBrace &&
          (!isalpha(pattern_[n]) && pattern_[n] != '{' &&
           pattern_[n] != '}')) {  // parse option
        option = pattern_.substr(i + 1, n - i - 1);
        break;
      }

      if (fmt_status == 0) {
        if (pattern_[n] == '{') {  // parse option
          option = pattern_.substr(i + 1, n - i - 1);
          fmt_status = kMeetOpenBrace;
          sub_format_begin = n;
          ++n;
          continue;
        }
      } else if (fmt_status == kMeetOpenBrace) {
        if (pattern_[n] == '}') {  // parse sub-format
          sub_format =
              pattern_.substr(sub_format_begin + 1, n - sub_format_begin - 1);
          fmt_status = kMeetCloseBrace;
          ++n;
          break;
        }
      }

      ++n;
      if (n == pattern_.size()) {  // parse option
        if (option.empty()) {
          option = pattern_.substr(i + 1);
        }
      }
    }

    if (fmt_status == 0) {
      if (!str.empty()) {  //  store string
        vec.push_back(std::make_tuple(str, std::string(), kString));
        str.clear();
      }

      // store option
      vec.push_back(std::make_tuple(option, sub_format, kOption));
      i = n - 1;
    } else if (fmt_status == kMeetOpenBrace) {  // pattern_'s illegal
      std::cout << "pattern parse error: " << pattern_ << " - "
                << pattern_.substr(i) << std::endl;
      vec.push_back(std::make_tuple("<<pattern_error>>", sub_format, kString));
    }
  }

  if (!str.empty()) {  // parse string
    vec.push_back(std::make_tuple(str, "", kString));
  }

  static std::map<std::string,
                  std::function<FormatItemPtr(const std::string &item_str)>>
      s_format_items = {
#define XX(str, C)                                                     \
  {                                                                    \
#str, [](const std::string                                         \
                 &item_str) { return FormatItemPtr(new C(item_str)); } \
  }
          XX(m, MsgItem),         // m:消息
          XX(p, LevelItem),       // p:日志级别
          XX(r, ElapseItem),      // r:累计毫秒数
          XX(c, NameItem),        // c:日志名称
          XX(t, ThreadIdItem),    // t:线程id
          XX(n, NewLineItem),     // n:换行
          XX(d, DateTimeItem),    // d:时间
          XX(f, FileNameItem),    // f:文件名
          XX(l, TabItem),         // l:行号
          XX(T, TabItem),         // T:Tab
          XX(F, FiberItem),       // F:协程id
          XX(N, ThreadNameItem),  // N:线程名称
#undef XX
      };

  for (auto &tuple_item : vec) {
    if (std::get<2>(tuple_item) == kString) {
      items_.push_back(FormatItemPtr(new StringItem(std::get<0>(tuple_item))));
    } else {
      auto it = s_format_items.find(std::get<0>(tuple_item));
      if (it == s_format_items.end()) {
        items_.push_back(FormatItemPtr(new StringItem(
            "<<error_format %" + std::get<0>(tuple_item) + ">>")));
      } else {
        items_.push_back(it->second(std::get<1>(tuple_item)));
      }
    }
  }
}

}  // namespace armsy
