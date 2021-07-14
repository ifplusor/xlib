#ifndef XLIB_THREAD_LATCH_LATCH_HPP_
#define XLIB_THREAD_LATCH_LATCH_HPP_

#include "xlib/__config.hpp"

#if __cpp_lib_latch >= 201907L && defined(XLIB_USE_STD_LATCH)

#include <latch>

namespace xlib {

using std::latch;

}  // namespace xlib

#else  // __cpp_lib_latch >= 201907L

#include <cassert>  // assert
#include <cstddef>  // std::ptrdiff_t

#include <atomic>     // std::atomic
#include <chrono>     // std::chrono::microseconds
#include <exception>  // std::make_exception_ptr
#include <future>     // std::future, std::promise
#include <stdexcept>  // std::runtime_error
#include <thread>     // std::this_thread::sleep_for

#include "xlib/utility/scope/scope_exit.hpp"

namespace xlib {

enum class latch_status { timeout, no_timeout };

class latch {
 public:
  explicit latch(std::ptrdiff_t value) : counter_(value), promise_(), future_(promise_.get_future()), waiting_(0) {
    assert(counter_ >= 0);
  }

  ~latch() {
    if (!try_wait()) {
      cancel_wait("latch is destructed");
    }

    while (waiting_ > 0) {
      std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
  }

  latch(const latch&) = delete;
  latch& operator=(const latch&) = delete;

  void count_down(std::ptrdiff_t n = 1) {
    if (try_count_down(n)) {
      awake_waiter();
    }
  }

  bool try_wait() const noexcept { return counter_ <= 0; }

  void wait() const {
    if (counter_ > 0) {
      waiting_++;
      auto release_waiting = make_scope_exit([this]() { waiting_--; });
      future_.wait();
    }
  }

  void arrive_and_wait(std::ptrdiff_t n = 1) {
    if (try_count_down(n)) {
      awake_waiter();
    } else {
      wait();
    }
  }

  template <typename Rep, typename Period>
  latch_status wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) const {
    if (counter_ > 0) {
      waiting_++;
      auto release_waiting = make_scope_exit([this]() { waiting_--; });
      if (future_.wait_for(timeout_duration) == std::future_status::timeout) {
        return latch_status::timeout;
      }
    }
    return latch_status::no_timeout;
  }

  template <typename Clock, typename Duration>
  latch_status wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) const {
    if (counter_ > 0) {
      waiting_++;
      auto release_waiting = make_scope_exit([this]() { waiting_--; });
      if (future_.wait_until(timeout_time) == std::future_status::timeout) {
        return latch_status::timeout;
      }
    }
    return latch_status::no_timeout;
  }

 private:
  bool try_count_down(std::ptrdiff_t n) {
    for (;;) {
      auto c = counter_.load();
      if (c <= 0) {
        return false;
      }
      auto nextc = c - n;
      if (counter_.compare_exchange_weak(c, nextc)) {
        return nextc <= 0;
      }
    }
  }

  void awake_waiter() { promise_.set_value(); }

  void cancel_wait(const char* reason) { promise_.set_exception(std::make_exception_ptr(std::runtime_error(reason))); }

 private:
  std::atomic<std::ptrdiff_t> counter_;
  std::promise<void> promise_;
  std::future<void> future_;

  mutable std::atomic<int> waiting_;
};

}  // namespace xlib

#endif  // __cpp_lib_latch >= 201907L

#endif  // XLIB_THREAD_LATCH_LATCH_HPP_
