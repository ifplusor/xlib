#ifndef XLIB_CONTAINER_CONQUEUE_UNBOUNDEDQUEUE_HPP_
#define XLIB_CONTAINER_CONQUEUE_UNBOUNDEDQUEUE_HPP_

#include <atomic>              // std::atomic
#include <condition_variable>  // std::condition_variable
#include <memory>              // std::unique_ptr
#include <mutex>               // std::mutex, std::unique_lock
#include <thread>              // std::this_thread
#include <utility>             // std::move

#include "xlib/container/conqueue/queue_op_status.hpp"

namespace xlib {

template <typename T>
class unbounded_queue;

namespace detail {

template <typename T>
struct unbounded_queue_node {
  using value_type = T;
  using type = unbounded_queue_node<value_type>;

  value_type value;
  std::atomic<type*> next;
};

}  // namespace detail

/**
 * @brief Unbounded concurrent queue
 *
 * @tparam T type
 *
 * @see https://wg21.link/P0260
 */
template <typename T>
class unbounded_queue {
 public:
  // Types:
  using value_type = T;
  using node_type = detail::unbounded_queue_node<value_type>;

 public:
  static bool is_lock_free() noexcept { return true; }

 public:
  unbounded_queue(bool clear_when_destruct = true)
      : sentinel_(static_cast<node_type*>(::operator new(sizeof(node_type)))) {
    sentinel_->next.store(sentinel_);
    head_ = tail_ = sentinel_;
  }

  ~unbounded_queue() {
    // clear this queue
    for (;;) {
      value_type value;
      if (try_pop(value) != queue_op_status::success) {
        break;
      }
    }

    ::operator delete(sentinel_);
  }

  // Disable copy
  unbounded_queue(const unbounded_queue&) = delete;
  unbounded_queue& operator=(const unbounded_queue&) = delete;

  // Disable move
  unbounded_queue(unbounded_queue&&) = delete;
  unbounded_queue& operator=(unbounded_queue&&) = delete;

  void push(const value_type& element) { push_impl(element); }
  void push(value_type&& element) { push_impl(std::move(element)); }

  value_type value_pop() {
    value_type element;
    if (try_pop(element) == queue_op_status::success) {
      return std::move(element);
    }
    std::unique_lock<std::mutex> lock(not_empty_mutex_);
    for (;;) {
      not_empty_cv_.wait(lock, [this]() { return !is_empty(); });
      auto status = try_pop(element);
      if (status == queue_op_status::success) {
        return std::move(element);
      }
    }
  }

  queue_op_status try_push(const value_type& element) {
    push_impl(element);
    return queue_op_status::success;
  }
  queue_op_status try_push(value_type&& element) {
    push_impl(std::move(element));
    return queue_op_status::success;
  }

  queue_op_status try_pop(value_type& element) {
    auto node = pop_impl();
    if (node == nullptr) {
      return queue_op_status::empty;
    }
    element = std::move(node->value);
    return queue_op_status::success;
  }

  queue_op_status wait_push(const value_type& element) {
    if (is_closed()) {
      return queue_op_status::closed;
    }
    return try_push(element);
  }
  queue_op_status wait_push(value_type&& element) {
    if (is_closed()) {
      return queue_op_status::closed;
    }
    return try_push(std::move(element));
  }

  queue_op_status wait_pop(value_type& element) {
    if (try_pop(element) == queue_op_status::success) {
      return queue_op_status::success;
    }
    std::unique_lock<std::mutex> lock(not_empty_mutex_);
    for (;;) {
      not_empty_cv_.wait(lock, [this]() { return is_closed() || !is_empty(); });
      if (is_closed()) {
        return queue_op_status::closed;
      }
      if (try_pop(element) == queue_op_status::success) {
        return queue_op_status::success;
      }
    }
  }

  void close() noexcept {
    closed_ = true;
    not_empty_cv_.notify_all();
  }

  void open() {}

  [[nodiscard]] bool is_closed() const noexcept { return closed_; }

  [[nodiscard]] bool is_empty() const noexcept { return sentinel_ == tail_.load(); }
  [[nodiscard]] bool is_full() const noexcept { return false; }

 private:
  void push_impl(value_type element) noexcept {
    auto* node = new node_type{std::move(element), sentinel_};
    auto* tail = tail_.exchange(node, std::memory_order_acq_rel);
    if (tail == sentinel_) {
      head_.store(node, std::memory_order_release);
    } else {
      // guarantee: tail is not released
      tail->next.store(node, std::memory_order_release);
    }
    not_empty_cv_.notify_one();
  }

  std::unique_ptr<node_type> pop_impl() noexcept {
    size_t i = 1;
    auto* head = head_.load(std::memory_order_acquire);
    for (;;) {
      if (head == sentinel_) {
        // no task, or it is/are not ready
        return nullptr;
      }
      if (head == nullptr) {
        if (0 == ++i % 15) {
          std::this_thread::yield();
        }
        head = head_.load(std::memory_order_acquire);
        continue;
      }
      if (head_.compare_exchange_weak(head, nullptr, std::memory_order_acq_rel)) {
        auto* next = head->next.load(std::memory_order_acquire);
        if (next == sentinel_) {
          auto* t = head;
          // only one element
          if (tail_.compare_exchange_strong(t, sentinel_, std::memory_order_release)) {
            t = nullptr;
            head_.compare_exchange_strong(t, sentinel_, std::memory_order_release);
            return std::unique_ptr<node_type>{head};
          }
          size_t j = 0;
          do {
            // push-pop conflict, spin
            if (0 == ++j % 10) {
              std::this_thread::yield();
            }
            next = head->next.load(std::memory_order_acquire);
          } while (next == sentinel_);
        }
        head_.store(next, std::memory_order_release);
        return std::unique_ptr<node_type>{head};
      }
    }
  }

  std::atomic<node_type*> head_, tail_;
  node_type* const sentinel_;

  std::mutex not_empty_mutex_;
  std::condition_variable not_empty_cv_;

  bool closed_{false};
};

}  // namespace xlib

#endif  // XLIB_CONTAINER_CONQUEUE_UNBOUNDEDQUEUE_HPP_
