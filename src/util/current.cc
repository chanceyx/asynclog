#include "current.h"

namespace current {

// __thread : each thread has its own t_cachedTid.
__thread uint32_t t_cached_tid = 0;

void cacheTid() {
  if (t_cached_tid == 0) {
    t_cached_tid = static_cast<pid_t>(::syscall(SYS_gettid));
  }
}

}  // namespace CurrentThread
