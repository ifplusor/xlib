// reference: asio/detail/thread_group.hpp
#ifndef XLIB_THREAD_GROUP_HPP
#define XLIB_THREAD_GROUP_HPP

#include "thread.hpp"

namespace xlib {

class thread_group {
 public:
  // Constructor initialises an empty thread group.
  thread_group() : first_(nullptr) {}
  thread_group(const std::string& name) : name_(name), first_(nullptr) {}

  template <typename Function>
  thread_group(const std::string& name, Function f, std::size_t num_threads) : name_(name), first_(nullptr) {
    create_threads(f, num_threads);
  }

  // Destructor joins any remaining threads in the group.
  ~thread_group() { join(); }

  // Create a new thread in the group.
  template <typename Function>
  void create_thread(Function f) {
    first_ = new item(name_, f, first_);
  }

  // Create new threads in the group.
  template <typename Function>
  void create_threads(Function f, std::size_t num_threads) {
    for (std::size_t i = 0; i < num_threads; ++i) {
      create_thread(f);
    }
  }

  void start() {
    auto* it = first_;
    while (it != nullptr) {
      it->start();
      it = it->next_;
    }
  }

  // Wait for all threads in the group to exit.
  void join() {
    while (first_) {
      first_->thread_.join();
      auto* tmp = first_;
      first_ = first_->next_;
      delete tmp;
    }
  }

 private:
  // Structure used to track a single thread in the group.
  struct item {
    template <typename Function>
    explicit item(const std::string& name, Function f, item* next) : thread_(name), next_(next) {
      thread_.set_target(f);
    }

    void start() { thread_.start(); }

    thread thread_;
    item* next_;
  };

 private:
  std::string name_;

  // The first thread in the group.
  item* first_;
};

}  // namespace xlib

#endif  // XLIB_THREAD_GROUP_HPP
