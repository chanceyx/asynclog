#include "current.h"

namespace current {

// __thread : each thread has its own t_cached_tid.
__thread uint64_t t_cached_tid = 0;

void cacheTid() {
  if (t_cached_tid == 0) {
#ifdef __linux__
    t_cached_tid = static_cast<pid_t>(::syscall(SYS_gettid));
#elif _WIN32
    t_cached_tid = GetCurrentThreadId();
#elif __APPLE__
    pthread_threadid_np(NULL, &t_cached_tid);
#endif
  }
}

}  // namespace current
