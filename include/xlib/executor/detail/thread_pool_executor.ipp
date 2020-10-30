#ifndef XLIB_EXECUTOR_DETAIL_THREADPOOLEXECUTOR_IPP_
#define XLIB_EXECUTOR_DETAIL_THREADPOOLEXECUTOR_IPP_

#include "xlib/container/concurrent_queue.hpp"
#include "xlib/executor/detail/abstract_executor_service.ipp"
#include "xlib/system/thread_group.hpp"

namespace xlib {

class thread_pool_executor : public abstract_executor_service {
 public:
  explicit thread_pool_executor(std::size_t num_threads, bool start_immediately = true)
      : task_queue_(), state_(STOP), num_threads_(num_threads), free_threads_(0) {
    if (start_immediately) {
      startup();
    }
  }
  explicit thread_pool_executor(const std::string& name, std::size_t num_threads, bool start_immediately = true)
      : task_queue_(), state_(STOP), num_threads_(num_threads), thread_group_(name), free_threads_(0) {
    if (start_immediately) {
      startup();
    }
  }

  virtual void startup() {
    if (state_ == STOP) {
      state_ = RUNNING;
      thread_group_.create_threads(std::bind(&thread_pool_executor::run, this), num_threads_);
      thread_group_.start();
    }
  }

  void shutdown(bool immediately = true) override {
    if (state_ == RUNNING) {
      state_ = immediately ? STOP : SHUTDOWN;
      wakeup_event_.notify_all();
      thread_group_.join();
      state_ = STOP;
    }
  }

  bool is_shutdown() override { return state_ != RUNNING; }

  std::size_t num_threads() { return num_threads_; }

 protected:
  static const unsigned int ACCEPT_NEW_TASKS = 1U << 0U;
  static const unsigned int PROCESS_QUEUED_TASKS = 1U << 1U;

  enum state { STOP = 0, SHUTDOWN = PROCESS_QUEUED_TASKS, RUNNING = ACCEPT_NEW_TASKS | PROCESS_QUEUED_TASKS };

  void execute(std::unique_ptr<executor_handler> command) override {
    if (state_ & ACCEPT_NEW_TASKS) {
      task_queue_.push_back(command.release());
      if (free_threads_ > 0) {
        wakeup_event_.notify_one();
      }
    } else {
      command->abort(std::logic_error("executor don't accept new tasks."));
    }
  }

  void run() {
    while (state_ & PROCESS_QUEUED_TASKS) {
      auto task = task_queue_.pop_front();
      if (task != nullptr) {
        task->operator()();
      } else {
        if (!(state_ & ACCEPT_NEW_TASKS)) {
          // don't accept new tasks
          break;
        }

        // wait new tasks
        std::unique_lock<std::mutex> lock(wakeup_mutex_);
        if (task_queue_.empty()) {
          free_threads_++;
          wakeup_event_.wait_for(lock, std::chrono::seconds(5));
          free_threads_--;
        }
      }
    }
  }

 protected:
  concurrent_queue<executor_handler> task_queue_;

 private:
  state state_;

  std::size_t num_threads_;
  thread_group thread_group_;

  std::mutex wakeup_mutex_;
  std::condition_variable wakeup_event_;
  std::atomic<int> free_threads_;
};

}  // namespace xlib

#endif  // XLIB_EXECUTOR_DETAIL_THREADPOOLEXECUTOR_IPP_
