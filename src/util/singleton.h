#ifndef _SINGLETION_
#define _SINGLETION_

#include <memory>

namespace asynclog {

// Disable the copy constructor of its derived class. The objects
// of classes inherit from noncopyable can not be copy.
class noncopyable {
 public:
  noncopyable(const noncopyable &) = delete;
  noncopyable *operator=(const noncopyable &) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

// Singletion use to get the single instance of type T.
template <typename T, typename X = void, int N = 0>
class Singletion {
 public:
  static T &GetInstance() {
    static T t;
    return t;
  }
};

// SingletionPtr use to get the single instance of type T*.
template <typename T, typename X = void, int N = 0>
class SingletonPtr {
 public:
  static std::shared_ptr<T> GetInstance() {
    static auto ptr = std::shared_ptr<T>(new T);
    return ptr;
  }
};

}  // namespace asynclog

#endif  //_SINGLETION_
