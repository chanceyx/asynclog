#include "thread.h"

#include <condition_variable>
#include <mutex>
#include <string>

#include "current.h"

namespace asynclog {

std::atomic_int Thread::thread_num_(0);

Thread::Thread(ThreadFunc func, const std::string &name)
    : started_(false),
      joined_(false),
      tid_(0),
      thread_func_(std::move(func)),
      name_(name) {
  setDefaultName();
}

Thread::~Thread() {
  if (started_ && !joined_) thread_->detach();
}

void Thread::start() {
  started_ = true;
  std::mutex mut;
  std::unique_lock<std::mutex> lock(mut);
  std::condition_variable cond;

  thread_ = std::shared_ptr<std::thread>(new std::thread([&] {
    {
      std::unique_lock<std::mutex> lock(mut);
      tid_ = current::tid();
      cond.notify_one();
    }
    thread_func_();
  }));

  cond.wait(lock, [&] {
    if (tid_ == 0)
      return false;
    else
      return true;
  });
}

void Thread::join() {
  joined_ = true;
  thread_->join();
}

void Thread::setDefaultName() {
  int num = ++thread_num_;
  if (name_.empty()) {
    char buf[32] = {0};
    snprintf(buf, sizeof buf, "Thread%d", num);
    name_ = buf;
  }
}

}  // namespace asynclog
