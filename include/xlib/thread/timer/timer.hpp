#ifndef XLIB_THREAD_TIMER_TIMER_HPP_
#define XLIB_THREAD_TIMER_TIMER_HPP_

#include <memory>

#include "xlib/execution/executor.hpp"

namespace xlib {

class timer {
 public:
  // Types:
  using task_type = executor::task_type;

 public:
  virtual ~timer() = default;

  virtual void timetable(std::chrono::steady_clock::time_point deadline, task_type&& task) = 0;

  template <typename Duration, typename Function, typename... Args>
  auto schedule(Duration d, Function&& f, Args&&... args) {
    using result_t = std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>;
    shared_packaged_task<result_t(Args...)> packaged_task{std::forward<Function>(f)};
    auto future = packaged_task.get_future();
    timetable(std::chrono::steady_clock::now() + d, std::move(packaged_task));
    return std::move(future);
  }
};

// class scheduled_executor_service : virtual public executor_service, virtual public scheduler {};

}  // namespace xlib

#endif  // XLIB_THREAD_TIMER_TIMER_HPP_
