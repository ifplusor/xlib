#ifndef XLIB_THREAD_SHAREDMUTEX_SHAREDLOCK_HPP_
#define XLIB_THREAD_SHAREDMUTEX_SHAREDLOCK_HPP_

#if _XLIB_STD_VER >= 2014L

#include <shared_mutex>

namespace xlib {

using std::shared_lock;

}

#else  // _XLIB_STD_VER >= 2014L

#include <chrono>
#include <mutex>

namespace xlib {

template <class Mutex>
class shared_lock {
 public:
  using mutex_type = Mutex;

  //
  // construct/copy/destroy

  shared_lock() noexcept = default;

  explicit shared_lock(mutex_type& m) : pm(&m), owns(true) { m.lock_shared(); }

  shared_lock(mutex_type& m, std::defer_lock_t /*unused*/) noexcept : pm(&m) {}

  shared_lock(mutex_type& m, std::try_to_lock_t /*unused*/) : pm(&m), owns(m.try_lock_shared()) {}

  shared_lock(mutex_type& m, std::adopt_lock_t /*unused*/) : pm(&m), owns(true) {}

  template <class Clock, class Duration>
  shared_lock(mutex_type& m, const std::chrono::time_point<Clock, Duration>& abs_time);

  template <class Rep, class Period>
  shared_lock(mutex_type& m, const std::chrono::duration<Rep, Period>& rel_time);

  ~shared_lock() {
    if (owns) {
      pm->unlock_shared();
    }
  }

  shared_lock(const shared_lock&) = delete;
  shared_lock& operator=(const shared_lock&) = delete;

  shared_lock(shared_lock&& u) noexcept : pm(u.pm), owns(u.owns) {
    u.pm = nullptr;
    u.owns = false;
  }
  shared_lock& operator=(shared_lock&& u) noexcept {
    pm = u.pm;
    owns = u.owns;
    u.pm = nullptr;
    u.owns = false;
  }

  //
  // locking

  void lock() {
    if (pm) {
      pm->lock_shared();
      owns = true;
    }
  }

  bool try_lock() {
    if (pm) {
      owns = pm->try_lock_shared();
    }
    return owns;
  }

  template <class Rep, class Period>
  bool try_lock_for(const std::chrono::duration<Rep, Period>& rel_time);

  template <class Clock, class Duration>
  bool try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time);

  void unlock() {
    if (pm && owns) {
      pm->unlock_shared();
    }
    owns = false;
  }

  //
  // modifiers

  void swap(shared_lock& u) noexcept {
    bool tmp_owns = owns;
    owns = u.owns;
    u.owns = tmp_owns;

    auto* tmp_pm = pm;
    pm = u.pm;
    u.pm = tmp_pm;
  }

  mutex_type* release() noexcept {
    auto* old_pm = pm;
    pm = nullptr;
    owns = false;
    return old_pm;
  }

  //
  // observers

  [[nodiscard]] bool owns_lock() const noexcept { return owns; }

  explicit operator bool() const noexcept { return owns_lock(); }

  mutex_type* mutex() const noexcept { return pm; }

 private:
  mutex_type* pm{nullptr};  // exposition only
  bool owns{false};         // exposition only
};

template <class Mutex>
void swap(shared_lock<Mutex>& x, shared_lock<Mutex>& y) noexcept {
  x.swap(y);
}

}  // namespace xlib

#endif  // _XLIB_STD_VER >= 2014L

#endif  // XLIB_THREAD_SHAREDMUTEX_SHAREDLOCK_HPP_
