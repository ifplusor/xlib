#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include <atomic>
#include <memory>
#include <utility>

template <typename T>
class concurrent_queue;

template <typename T>
class concurrent_queue_node {
 public:
  // types:
  typedef T value_type;
  typedef concurrent_queue_node<value_type> type;

  /**
   * return shared_ptr which wrap the value, and invalid the object.
   */
  std::shared_ptr<value_type> return_ptr() { return std::shared_ptr<value_type>(new value_type(std::move(value_))); }

 private:
  explicit concurrent_queue_node(const value_type& v) : value_(v){};
  explicit concurrent_queue_node(value_type&& v) : value_(std::move(v)){};

  value_type value_;
  type* volatile next_;
  type* volatile last_;

  friend concurrent_queue<value_type>;
};

template <typename T>
class concurrent_queue {
 public:
  // types:
  typedef T value_type;
  typedef concurrent_queue_node<value_type> node_type;

  concurrent_queue() { head_ = tail_ = nullptr; }

  void push_back(const value_type& v) {
    auto node = new node_type(v);
    push_back_impl(node);
  }

  void push_back(value_type&& v) {
    auto node = new node_type(std::move(v));
    push_back_impl(node);
  }

  std::shared_ptr<value_type> pop_front() {
    node_type* node = pop_front_impl();
    if (nullptr == node) {
      return std::shared_ptr<value_type>();
    } else {
      auto val = node->return_ptr();
      delete node;
      return val;
    }
  }

 private:
  void push_back_impl(node_type* node) {
    node->next_ = nullptr;
    for (;;) {
      node_type* tail = tail_.load();
      node->last_ = tail;
      if (nullptr == tail) {
        // insert head
        if (head_.compare_exchange_weak(tail, node)) {
          tail_.store(node);
          return;
        }
      } else {
        // insert tail
        if (tail_.compare_exchange_weak(tail, node)) {
          tail->next_ = node;
          return;
        }
      }
    }
  }

  node_type* pop_front_impl() {
    for (;;) {
      node_type* tail = tail_.load();
      node_type* head = head_.load();
      if (nullptr == tail) {
        // empty
        return nullptr;
      } else if (head == tail) {
        // only one element
        if (tail_.compare_exchange_weak(tail, nullptr)) {
          head_.store(nullptr);
          return head;
        }
      } else {
        node_type* next = head->next_;
        if (nullptr != next && head_.compare_exchange_weak(head, next)) {
          next->last_ = nullptr;
          return head;
        }
      }
    }
  }

  std::atomic<node_type*> head_, tail_;
};

#endif  // CONCURRENT_QUEUE_H
