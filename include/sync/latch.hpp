#ifndef XLIB_LATCH_HPP
#define XLIB_LATCH_HPP

#include <atomic>
#include <cassert>
#include <cstddef>
#include <future>
#include <thread>

#include "../time.hpp"

namespace xlib {

class latch {
 public:
  explicit latch(ptrdiff_t value) : count_(value), promise_(), future_(promise_.get_future()), waiting_(0) {
    assert(count_ >= 0);
  }

  ~latch() {
    if (!is_ready()) {
      cancel_wait("latch is destructed");
    }

    while (waiting_ > 0) {
      std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
  }

  latch(const latch&) = delete;
  latch& operator=(const latch&) = delete;

  void count_down_and_wait() {
    if (try_count_down(1)) {
      awake_waiter();
    } else {
      wait();
    }
  }

  void count_down(ptrdiff_t n = 1) {
    if (try_count_down(n)) {
      awake_waiter();
    }
  }

  bool is_ready() const noexcept { return count_ <= 0; }

  void wait() const {
    if (count_ > 0) {
      waiting_++;
      future_.wait();
      waiting_--;
    }
  }

  void wait(long timeout, time_unit unit) const {
    if (count_ > 0) {
      waiting_++;
      auto time_point = until_time_point(timeout, unit);
      future_.wait_until(time_point);
      waiting_--;
    }
  }

 private:
  bool try_count_down(ptrdiff_t n) {
    for (;;) {
      auto c = count_.load();
      if (c <= 0) {
        return false;
      }
      auto nextc = c - n;
      if (count_.compare_exchange_weak(c, nextc)) {
        return nextc <= 0;
      }
    }
  }

  void awake_waiter() {
    try {
      promise_.set_value();
    } catch (...) {
    }
  }

  void cancel_wait(const char* reason) {
    try {
      throw std::runtime_error(reason);
    } catch (...) {
      try {
        promise_.set_exception(std::current_exception());
      } catch (...) {
      }
    }
  }

 private:
  std::atomic<ptrdiff_t> count_;
  std::promise<void> promise_;
  std::future<void> future_;

  mutable std::atomic<int> waiting_;
};

}  // namespace xlib

#endif  // XLIB_LATCH_HPP
