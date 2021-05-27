#ifndef XLIB_EXECUTION_THREADPOOL_STATICTHREADPOOL_HPP_
#define XLIB_EXECUTION_THREADPOOL_STATICTHREADPOOL_HPP_

#include <cstddef>     // std::size_t
#include <functional>  // std::function

#include "xlib/execution/context/task_queue.hpp"
#include "xlib/thread/thread/thread_group.hpp"

namespace xlib {

namespace detail {

class __static_thread_pool : public execution::detail::__task_queue<>::context_base<__static_thread_pool> {
  using base_type = execution::detail::__task_queue<>::context_base<__static_thread_pool>;

 public:
  explicit __static_thread_pool(std::size_t num_threads, bool start_immediately = true) : num_threads_(num_threads) {
    if (start_immediately) {
      start();
    }
  }
  explicit __static_thread_pool(const std::string& name, std::size_t num_threads, bool start_immediately = true)
      : num_threads_(num_threads), thread_group_(name) {
    if (start_immediately) {
      start();
    }
  }

  // Disable copy
  __static_thread_pool(const __static_thread_pool&) = delete;
  __static_thread_pool& operator=(const __static_thread_pool&) = delete;

  // Disable move
  __static_thread_pool(__static_thread_pool&&) = delete;
  __static_thread_pool& operator=(__static_thread_pool&&) = delete;

  // stop accepting incoming work and wait for work to drain
  ~__static_thread_pool() {
    stop();
    wait();
  }

  void start() {
    if (state_ == state::stopped) {
      thread_group_.create_threads([this]() { run_forever(); }, num_threads_);
      thread_group_.start();
      state_ = state::running;
    }
  }

  // attach current thread to the thread pools list of worker threads
  void attach() {
    if (state_ == state::running) {
      run_forever();
    }
  }

  // signal all work to complete
  void stop() {
    if (state_ == state::running) {
      state_ = state::stopping;
      base_type::stop();
    }
  }

  // wait for all threads in the thread pool to complete
  void wait() {
    if (state_ == state::stopping) {
      thread_group_.join();
      state_ = state::stopped;
    }
  }

  bool is_shutdown() { return state_ != state::running; }

  [[nodiscard]] std::size_t num_threads() const { return num_threads_; }
  void set_num_threads(std::size_t num_threads) { num_threads_ = num_threads; }

 private:
  enum class state { running, stopping, stopped };
  state state_{state::stopped};
  std::size_t num_threads_;
  thread_group thread_group_;
};

}  // namespace detail

using static_thread_pool = detail::__static_thread_pool;

}  // namespace xlib

#endif  // XLIB_EXECUTION_THREADPOOL_STATICTHREADPOOL_HPP_
