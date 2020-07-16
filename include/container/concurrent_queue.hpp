#ifndef XLIB_CONCURRENT_QUEUE_HPP
#define XLIB_CONCURRENT_QUEUE_HPP

#include <atomic>
#include <memory>
#include <utility>

namespace xlib {

template <typename T>
class concurrent_queue;

template <typename T>
class concurrent_queue_node {
 public:
  // types:
  typedef T value_type;
  typedef concurrent_queue_node<value_type> type;

 private:
  template <typename E,
            typename std::enable_if<std::is_same<typename std::decay<E>::type, value_type>::value, int>::type = 0>
  explicit concurrent_queue_node(E&& v) : value_(new value_type(std::forward<E>(v))) {}

  // for pointer
  template <class E, typename std::enable_if<std::is_convertible<E, value_type*>::value, int>::type = 0>
  explicit concurrent_queue_node(E v) : value_(v) {}

  value_type* value_;
  type* volatile next_;

  friend concurrent_queue<value_type>;
};

template <typename T>
class concurrent_queue {
 public:
  // types:
  typedef T value_type;
  typedef concurrent_queue_node<value_type> node_type;

  ~concurrent_queue() {
    // clear this queue
    while (_clear_when_destruct) {
      if (nullptr == pop_front()) {
        break;
      }
    }

    delete[](char*) sentinel;
  }

  concurrent_queue(bool clear_when_destruct = true)
      : sentinel((node_type*)new char[sizeof(node_type)]), _clear_when_destruct(clear_when_destruct) {
    sentinel->next_ = sentinel;
    head_ = tail_ = sentinel;
  }

  bool empty() { return sentinel == tail_.load(); }

  template <class E>
  void push_back(E&& v) {
    auto* node = new node_type(std::forward<E>(v));
    push_back_impl(node);
  }

  std::unique_ptr<value_type> pop_front() {
    node_type* node = pop_front_impl();
    if (node == sentinel) {
      return std::unique_ptr<value_type>();
    } else {
      auto val = node->value_;
      delete node;
      return std::unique_ptr<value_type>(val);
    }
  }

 private:
  void push_back_impl(node_type* node) noexcept {
    node->next_ = sentinel;
    auto tail = tail_.exchange(node);
    if (tail == sentinel) {
      head_.store(node);
    } else {
      // guarantee: tail is not released
      tail->next_ = node;
    }
  }

  node_type* pop_front_impl() noexcept {
    auto head = head_.load();
    for (size_t i = 1;; i++) {
      if (head == sentinel) {
        // no task, or it is/are not ready
        return sentinel;
      }
      if (head != nullptr) {
        if (head_.compare_exchange_weak(head, nullptr)) {
          auto next = head->next_;
          if (next == sentinel) {
            auto t = head;
            // only one element
            if (tail_.compare_exchange_strong(t, sentinel)) {
              t = nullptr;
              head_.compare_exchange_strong(t, sentinel);
              return head;
            }
            size_t j = 0;
            do {
              // push-pop conflict, spin
              if (0 == j++ % 10) {
                std::this_thread::yield();
              }
              next = head->next_;
            } while (next == sentinel);
          }
          head_.store(next);
          return head;
        }
      } else {
        head = head_.load();
      }
      if (0 == i % 15 && head != sentinel) {
        std::this_thread::yield();
        head = head_.load();
      }
    }
  }

  std::atomic<node_type*> head_, tail_;
  node_type* const sentinel;
  bool _clear_when_destruct;
};

}  // namespace xlib

#endif  // XLIB_CONCURRENT_QUEUE_HPP
