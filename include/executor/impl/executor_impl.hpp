#ifndef XLIB_EXECUTOR_IMPL_HPP
#define XLIB_EXECUTOR_IMPL_HPP

#include <atomic>
#include <memory>
#include <queue>

#include "../../container/concurrent_queue.hpp"
#include "../../system/thread_group.hpp"
#include "../../time.hpp"

namespace xlib {

class abstract_executor_service : virtual public executor_service {
 public:
  std::future<void> submit(const handler_type& task) override {
    executor_handler handler(task);
    std::future<void> fut = handler.promise_->get_future();
    execute(std::move(handler));
    return fut;
  }
};

class thread_pool_executor : public abstract_executor_service {
 public:
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

  void shutdown() override {
    if (state_ == RUNNING) {
      state_ = SHUTDOWN;
      wakeup_event_.notify_all();
      thread_group_.join();
      state_ = STOP;
    }
  }

  bool is_shutdown() override { return state_ != RUNNING; }

 protected:
  static const unsigned int ACCEPT_NEW_TASKS = 1U << 0U;
  static const unsigned int PROCESS_QUEUED_TASKS = 1U << 1U;

  enum state { STOP = 0, SHUTDOWN = PROCESS_QUEUED_TASKS, RUNNING = ACCEPT_NEW_TASKS | PROCESS_QUEUED_TASKS };

  void execute(executor_handler&& command) override {
    if (state_ & ACCEPT_NEW_TASKS) {
      task_queue_.push_back(std::forward<executor_handler>(command));
      if (free_threads_ > 0) {
        wakeup_event_.notify_one();
      }
    } else {
      command.abort(std::logic_error("executor don't accept new tasks."));
    }
  }

  void run() {
    while (state_ & PROCESS_QUEUED_TASKS) {
      auto task = task_queue_.pop_front();
      if (task) {
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

struct scheduled_executor_handler : public executor_handler {
  std::chrono::steady_clock::time_point wakeup_time_;

  explicit scheduled_executor_handler(const handler_type& handler, const std::chrono::steady_clock::time_point& time)
      : executor_handler(handler), wakeup_time_(time) {}

  bool operator<(const scheduled_executor_handler& other) const { return (wakeup_time_ > other.wakeup_time_); }
};

class scheduled_thread_pool_executor : public thread_pool_executor, virtual public scheduled_executor_service {
 public:
  explicit scheduled_thread_pool_executor(const std::string& name,
                                          std::size_t num_threads,
                                          bool start_immediately = true)
      : thread_pool_executor(name, num_threads, false),
        stopped_(true),
        single_thread_(false),
        timer_thread_(name + "-Timer") {
    timer_thread_.set_target(&scheduled_thread_pool_executor::time_daemon, this);
    if (start_immediately) {
      startup();
    }
  }

  explicit scheduled_thread_pool_executor(const std::string& name, bool start_immediately = true)
      : thread_pool_executor(name, 0, false), stopped_(true), single_thread_(true), timer_thread_(name + "-Timer") {
    timer_thread_.set_target(&scheduled_thread_pool_executor::time_daemon, this);
    if (start_immediately) {
      startup();
    }
  }

  void startup() override {
    if (stopped_) {
      stopped_ = false;

      // startup task threads
      if (!single_thread_) {
        thread_pool_executor::startup();
      }

      // start time daemon
      timer_thread_.start();
    }
  }

  void shutdown() override {
    if (!stopped_) {
      stopped_ = true;

      time_event_.notify_one();
      timer_thread_.join();

      if (!single_thread_) {
        thread_pool_executor::shutdown();
      }
    }
  }

  bool is_shutdown() override { return stopped_; }

  std::future<void> submit(const handler_type& task) override {
    if (!single_thread_) {
      return thread_pool_executor::submit(task);
    } else {
      return schedule(task, 0, time_unit::milliseconds);
    }
  }

  std::future<void> schedule(const handler_type& task, long delay, time_unit unit) override {
    auto time_point = until_time_point(delay, unit);
    scheduled_executor_handler handler(task, time_point);
    std::future<void> fut = handler.promise_->get_future();

    {
      std::unique_lock<std::mutex> lock(time_mutex_);
      if (time_queue_.empty() || time_queue_.top().wakeup_time_ < time_point) {
        time_queue_.push(std::move(handler));
        time_event_.notify_one();
      } else {
        time_queue_.push(std::move(handler));
      }
    }

    return fut;
  }

 protected:
  void time_daemon() {
    std::unique_lock<std::mutex> lock(time_mutex_);
    while (!stopped_) {
      auto now = std::chrono::steady_clock::now();
      while (!time_queue_.empty()) {
        auto& top = const_cast<scheduled_executor_handler&>(time_queue_.top());
        if (top.wakeup_time_ <= now) {
          if (!single_thread_) {
            thread_pool_executor::execute(std::move(top));
            time_queue_.pop();
          } else {
            auto copy = top;
            time_queue_.pop();
            lock.unlock();
            copy();
            lock.lock();

            // if function cost more time, we need re-watch clock
            now = std::chrono::steady_clock::now();
          }
        } else {
          break;
        }
      }

      if (!time_queue_.empty()) {
        auto& top = time_queue_.top();
        time_event_.wait_for(lock, top.wakeup_time_ - now);
      } else {
        // default, wakeup after 10 seconds for check stopped flag.
        time_event_.wait_for(lock, std::chrono::seconds(10));
      }
    }
  }

 protected:
  std::priority_queue<scheduled_executor_handler> time_queue_;

 private:
  bool stopped_;

  bool single_thread_;
  thread timer_thread_;

  std::mutex time_mutex_;
  std::condition_variable time_event_;
};

}  // namespace xlib

#endif  // XLIB_EXECUTOR_IMPL_HPP
