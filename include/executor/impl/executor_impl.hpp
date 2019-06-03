#ifndef EXECUTOR_IMPL_H
#define EXECUTOR_IMPL_H

#include <atomic>
#include <memory>
#include <queue>

#include "../../container/concurrent_queue.hpp"
#include "../../system/thread_group.hpp"

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
  virtual ~thread_pool_executor() { shutdown(); }

  explicit thread_pool_executor(std::size_t num_threads, bool start_immediately = true)
      : state_(STOP), num_threads_(num_threads), free_threads_(0) {
    if (start_immediately) {
      startup();
    }
  }

  virtual void startup() {
    if (state_ == STOP) {
      state_ = RUNNING;
      thread_group_.create_threads(std::bind(&thread_pool_executor::run, this), num_threads_);
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
        free_threads_++;
        wakeup_event_.wait_for(lock, std::chrono::seconds(30));
        free_threads_--;
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

class scheduled_thread_pool_executor : public thread_pool_executor, virtual public scheduled_executor {
 public:
  explicit scheduled_thread_pool_executor(std::size_t num_threads, bool start_immediately = true)
      : thread_pool_executor(num_threads == 1 ? 0 : num_threads, false),
        stopped_(true),
        single_thread_(num_threads == 1),
        time_thread_(nullptr) {
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

      // startup time daemon
      time_thread_ = new std::thread(&scheduled_thread_pool_executor::time_daemon, this);
    }
  }

  void shutdown() override {
    if (!stopped_) {
      stopped_ = true;

      time_event_.notify_one();
      time_thread_->join();
      time_thread_ = nullptr;

      if (!single_thread_) {
        thread_pool_executor::shutdown();
      }
    }
  }

  bool is_shutdown() override { return stopped_; }

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
            execute(std::move(top));
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
  std::thread* time_thread_;

  std::mutex time_mutex_;
  std::condition_variable time_event_;
};

#endif  // EXECUTOR_IMPL_H
