#ifndef XLIB_MEMORY_REFCOUNT_HPP_
#define XLIB_MEMORY_REFCOUNT_HPP_

#include <cstdint>

#include <atomic>

#include "xlib/memory/noncopyable.hpp"

namespace xlib {

class refcount : public noncopyable {
 public:
  refcount() : count_(1) {}

  inline int32_t operator*() { return count_.load(std::memory_order_acquire); }

  inline int32_t operator++() { return count_.fetch_add(1, std::memory_order_relaxed) + 1; }

  inline int32_t operator++(int) { return count_.fetch_add(1, std::memory_order_relaxed); }

  inline int32_t operator--() {
    auto count = count_.fetch_sub(1, std::memory_order_relaxed);
    if (count == 1) {
      delete this;
    }
    return count - 1;
  }

  inline int32_t operator--(int) {
    auto count = count_.fetch_sub(1, std::memory_order_relaxed);
    if (count == 1) {
      delete this;
    }
    return count;
  }

 private:
  std::atomic<int32_t> count_;
};

}  // namespace xlib

#endif  // XLIB_MEMORY_REFCOUNT_HPP_
