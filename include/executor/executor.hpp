#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <chrono>
#include <functional>
#include <future>
#include <utility>

typedef std::function<void()> handler_type;

struct executor_handler {
  handler_type handler_;
  std::shared_ptr<std::promise<void>> promise_;

  explicit executor_handler(handler_type handler) : handler_(std::move(handler)), promise_(new std::promise<void>) {}
  explicit executor_handler(handler_type&& handler) : handler_(std::move(handler)), promise_(new std::promise<void>) {}

  void operator()() noexcept {
    // call handler, then set promise
    if (promise_) {
      try {
        // handler that may throw
        handler_();
        promise_->set_value();
      } catch (...) {
        try {
          // store anything thrown in the promise
          promise_->set_exception(std::current_exception());
        } catch (...) {
        }  // set_exception() may throw too
      }
    }
  }

  template <class _Ep>
  void abort(_Ep&& exception) noexcept {
    if (promise_) {
      promise_->set_exception(std::make_exception_ptr(std::forward<_Ep>(exception)));
    }
  }
};

class executor {
 public:
  virtual void execute(const executor_handler& command) {
    // pass rvalue
    auto copy = command;
    execute(std::move(copy));
  }
  virtual void execute(executor_handler&& command) {
    // pass lvalue;
    execute(command);
  };
};

class executor_service : virtual public executor {
 public:
  virtual void shutdown() = 0;
  virtual bool is_shutdown() = 0;
  virtual std::future<void> submit(const handler_type& task) = 0;
};

enum time_unit { nanoseconds, microseconds, milliseconds, seconds, minutes, hours };

std::chrono::steady_clock::time_point until_time_point(long delay, time_unit unit) {
  auto now = std::chrono::steady_clock::now();
  switch (unit) {
    case nanoseconds:
      return now + std::chrono::nanoseconds(delay);
    case microseconds:
      return now + std::chrono::microseconds(delay);
    case milliseconds:
      return now + std::chrono::milliseconds(delay);
    case seconds:
      return now + std::chrono::seconds(delay);
    case minutes:
      return now + std::chrono::minutes(delay);
    case hours:
      return now + std::chrono::hours(delay);
  }
}

class scheduled_executor : virtual public executor_service {
  virtual std::future<void> schedule(const handler_type& task, long delay, time_unit unit) = 0;
};

#include "impl/executor_impl.hpp"

#endif  // EXECUTOR_H
