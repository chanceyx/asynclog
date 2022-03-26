#pragma once

#include <stdint.h>
#include <sys/syscall.h>
#include <unistd.h>

// CurrentThread is used to get and catch the current thread information. Each
// log event has a tid_ member, if each log event use system call to get their
// tid, too many systems call may affect the performance of the log service. So
// cache each thread's information may be a good idea to get better performance.
namespace CurrentThread {

// variables declared by keyword __thread can have their own independent values
// in each thread.
extern __thread uint32_t t_cached_tid;         // thread id
extern __thread char t_tid_string[32];         // thread id in string
extern __thread uint32_t t_tid_string_length;  // thread id's length in string
extern __thread const char* t_thread_name;     // thread's name

void cacheTid();

inline uint32_t tid() {
  if (__builtin_expect(t_cached_tid == 0, 0)) {
    cacheTid();
  }
  return t_cached_tid;
}

inline uint32_t tidStringLength() { return t_tid_string_length; }

inline const char* tidString() { return t_tid_string; }

inline const char* threadName() { return t_thread_name; }

}  // namespace CurrentThread
