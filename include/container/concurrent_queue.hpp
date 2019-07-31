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
  explicit concurrent_queue_node(const value_type& v) : value_(new value_type(v)){};
  explicit concurrent_queue_node(value_type&& v) : value_(new value_type(std::move(v))){};

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

  ~concurrent_queue() { delete[](char*) sentinel; }

  concurrent_queue() : sentinel((node_type*)new char[sizeof(node_type)]) {
    sentinel->next_ = sentinel;
    head_ = tail_ = sentinel;
  }

  bool empty() { return sentinel == tail_.load(); }

  void push_back(const value_type& v) {
    auto* node = new node_type(v);
    push_back_impl(node);
  }

  void push_back(value_type&& v) {
    auto* node = new node_type(std::move(v));
    push_back_impl(node);
  }

  std::shared_ptr<value_type> pop_front() {
    node_type* node = pop_front_impl();
    if (node == sentinel) {
      return std::shared_ptr<value_type>();
    } else {
      auto val = node->value_;
      delete node;
      return std::shared_ptr<value_type>(val);
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
    for (;;) {
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
            do {
              // push-pop conflict, spin
              next = head->next_;
            } while (next == sentinel);
          }
          head_.store(next);
          return head;
        }
      } else {
        head = head_.load();
      }
    }
  }

  std::atomic<node_type*> head_, tail_;
  node_type* const sentinel;
};

}  // namespace xlib

#endif  // XLIB_CONCURRENT_QUEUE_HPP
