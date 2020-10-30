#ifndef XLIB_EXECUTOR_DETAIL_SCHEDULEDTHREADPOOLEXECUTOR_IPP_
#define XLIB_EXECUTOR_DETAIL_SCHEDULEDTHREADPOOLEXECUTOR_IPP_

#include <queue>

#include "xlib/executor/detail/thread_pool_executor.ipp"

namespace xlib {

struct scheduled_executor_handler : public executor_handler {
  std::chrono::steady_clock::time_point wakeup_time_;

  template <typename H,
            typename std::enable_if<std::is_same<typename std::decay<H>::type, handler_type>::value, int>::type = 0>
  explicit scheduled_executor_handler(H&& handler, const std::chrono::steady_clock::time_point& time)
      : executor_handler(std::forward<handler_type>(handler)), wakeup_time_(time) {}

  bool operator<(const scheduled_executor_handler& other) const { return (wakeup_time_ > other.wakeup_time_); }

  static bool less(const std::unique_ptr<scheduled_executor_handler>& a,
                   const std::unique_ptr<scheduled_executor_handler>& b) {
    return *a < *b;
  }
};

class scheduled_thread_pool_executor : public thread_pool_executor, virtual public scheduled_executor_service {
 public:
  explicit scheduled_thread_pool_executor(std::size_t thread_nums, bool start_immediately = true)
      : thread_pool_executor(thread_nums, false),
        time_queue_(&scheduled_executor_handler::less),
        stopped_(true),
        single_thread_(false),
        timer_thread_() {
    timer_thread_.set_target(&scheduled_thread_pool_executor::time_daemon, this);
    if (start_immediately) {
      startup();
    }
  }
  explicit scheduled_thread_pool_executor(const std::string& name,
                                          std::size_t thread_nums,
                                          bool start_immediately = true)
      : thread_pool_executor(name, thread_nums, false),
        time_queue_(&scheduled_executor_handler::less),
        stopped_(true),
        single_thread_(false),
        timer_thread_(name + "-Timer") {
    timer_thread_.set_target(&scheduled_thread_pool_executor::time_daemon, this);
    if (start_immediately) {
      startup();
    }
  }

  explicit scheduled_thread_pool_executor(bool start_immediately = true)
      : thread_pool_executor(0, false),
        time_queue_(&scheduled_executor_handler::less),
        stopped_(true),
        single_thread_(true),
        timer_thread_() {
    timer_thread_.set_target(&scheduled_thread_pool_executor::time_daemon, this);
    if (start_immediately) {
      startup();
    }
  }
  explicit scheduled_thread_pool_executor(const std::string& name, bool start_immediately = true)
      : thread_pool_executor(name, 0, false),
        time_queue_(&scheduled_executor_handler::less),
        stopped_(true),
        single_thread_(true),
        timer_thread_(name + "-Timer") {
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

  void shutdown(bool immediately = true) override {
    if (!stopped_) {
      stopped_ = true;

      time_event_.notify_one();
      timer_thread_.join();

      if (!single_thread_) {
        thread_pool_executor::shutdown(immediately);
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
    std::unique_ptr<scheduled_executor_handler> handler(
        new scheduled_executor_handler(const_cast<handler_type&>(task), time_point));
    std::future<void> fut = handler->promise_->get_future();

    {
      std::unique_lock<std::mutex> lock(time_mutex_);
      if (time_queue_.empty() || time_queue_.top()->wakeup_time_ < time_point) {
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
        auto& top = const_cast<std::unique_ptr<scheduled_executor_handler>&>(time_queue_.top());
        if (top->wakeup_time_ <= now) {
          if (!single_thread_) {
            thread_pool_executor::execute(std::move(top));
            time_queue_.pop();
          } else {
            auto copy = std::move(top);
            time_queue_.pop();
            lock.unlock();
            (*copy)();
            lock.lock();

            // if function cost more time, we need re-watch clock
            now = std::chrono::steady_clock::now();
          }
        } else {
          break;
        }
      }

      if (!time_queue_.empty()) {
        const auto& top = time_queue_.top();
        // wait more 10 milliseconds
        time_event_.wait_for(lock, top->wakeup_time_ - now + std::chrono::milliseconds(10));
      } else {
        // default, wakeup after 10 seconds for check stopped flag.
        time_event_.wait_for(lock, std::chrono::seconds(10));
      }
    }
  }

 protected:
  std::priority_queue<std::unique_ptr<scheduled_executor_handler>,
                      std::vector<std::unique_ptr<scheduled_executor_handler>>,
                      std::function<bool(const std::unique_ptr<scheduled_executor_handler>&,
                                         const std::unique_ptr<scheduled_executor_handler>&)>>
      time_queue_;

 private:
  bool stopped_;

  bool single_thread_;
  thread timer_thread_;

  std::mutex time_mutex_;
  std::condition_variable time_event_;
};

}  // namespace xlib

#endif  // XLIB_EXECUTOR_DETAIL_SCHEDULEDTHREADPOOLEXECUTOR_IPP_
