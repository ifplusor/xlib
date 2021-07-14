#ifndef XLIB_THREAD_SHAREDMUTEX_SHAREDMUTEX_HPP_
#define XLIB_THREAD_SHAREDMUTEX_SHAREDMUTEX_HPP_

#include "xlib/__config.hpp"

#if __cpp_lib_shared_mutex >= 201505L

#include <shared_mutex>

namespace xlib {

using std::shared_mutex;

}

#else  // __cpp_lib_shared_mutex >= 201505L

#include <pthread.h>

namespace xlib {

class shared_mutex {
 public:
  // Types:
  using native_handle_type = pthread_rwlock_t*;

 public:
  shared_mutex() { ::pthread_rwlock_init(&mutex_, NULL); }
  ~shared_mutex() { ::pthread_rwlock_destroy(&mutex_); }

  shared_mutex(const shared_mutex&) = delete;
  shared_mutex& operator=(const shared_mutex&) = delete;

  shared_mutex(shared_mutex&&) = delete;
  shared_mutex& operator=(shared_mutex&&) = delete;

  // exclusive ownership
  void lock() { ::pthread_rwlock_wrlock(&mutex_); }
  bool try_lock() { return ::pthread_rwlock_trywrlock(&mutex_) == 0; }
  void unlock() { ::pthread_rwlock_unlock(&mutex_); }

  // shared ownership
  void lock_shared() { ::pthread_rwlock_rdlock(&mutex_); }
  bool try_lock_shared() { return ::pthread_rwlock_tryrdlock(&mutex_) == 0; }
  void unlock_shared() { ::pthread_rwlock_unlock(&mutex_); }

  native_handle_type native_handle() { return &mutex_; }

 private:
  pthread_rwlock_t mutex_;
};

}  // namespace xlib

#endif  // __cpp_lib_shared_mutex >= 201505L

#endif  // XLIB_THREAD_SHAREDMUTEX_SHAREDMUTEX_HPP_
