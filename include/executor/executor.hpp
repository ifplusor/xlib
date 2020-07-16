#ifndef XLIB_EXECUTOR_HPP
#define XLIB_EXECUTOR_HPP

#include <functional>
#include <future>
#include <utility>

#include "../time.hpp"

namespace xlib {

typedef std::function<void()> handler_type;

struct executor_handler {
  const handler_type handler_;
  std::unique_ptr<std::promise<void>> promise_;

  template <typename H,
            typename std::enable_if<std::is_same<typename std::decay<H>::type, handler_type>::value, int>::type = 0>
  explicit executor_handler(H&& handler)
      : handler_(std::forward<handler_type>(handler)), promise_(new std::promise<void>) {}

  void operator()() noexcept {
    // call handler, then set promise
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

  template <class E>
  void abort(E&& exception) noexcept {
    promise_->set_exception(std::make_exception_ptr(std::forward<E>(exception)));
  }

 private:
  executor_handler() = delete;
};

class executor {
 public:
  virtual ~executor() = default;

  virtual void execute(std::unique_ptr<executor_handler> command) = 0;
};

class executor_service : virtual public executor {
 public:
  virtual void shutdown(bool immediately) = 0;
  virtual bool is_shutdown() = 0;
  virtual std::future<void> submit(const handler_type& task) = 0;
};

class scheduled_executor_service : virtual public executor_service {
 public:
  virtual std::future<void> schedule(const handler_type& task, long delay, time_unit unit) = 0;
};

}  // namespace xlib

#include "impl/executor_impl.hpp"

#endif  // XLIB_EXECUTOR_HPP
