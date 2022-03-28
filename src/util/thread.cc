#include "thread.h"

#include <semaphore.h>

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
  sem_t sem;
  sem_init(&sem, false, 0);

  thread_ = std::shared_ptr<std::thread>(new std::thread([&] {
    tid_ = current::tid();
    sem_post(&sem);
    thread_func_();
  }));

  sem_wait(&sem);
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
