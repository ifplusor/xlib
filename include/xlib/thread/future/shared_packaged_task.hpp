#ifndef XLIB_THREAD_FUTURE_SHAREDPACKAGETASK_HPP_
#define XLIB_THREAD_FUTURE_SHAREDPACKAGETASK_HPP_

#include <future>   // std::future, std::packaged_task
#include <memory>   // std::shared_ptr
#include <utility>  // std::forward

namespace xlib {

template <typename Callable>
class shared_packaged_task;

/**
 * The warpper of packaged_task.
 *
 * shared_packaged_task satisfies the requirements of CopyConstructible and CopyAssignable.
 */
template <typename R, typename... Args>
class shared_packaged_task<R(Args...)> {
 public:
  // Types:
  using result_type = R;

 public:
  template <typename Function>
  explicit shared_packaged_task(Function&& f)
      : packaged_task_{std::make_shared<std::packaged_task<R(Args...)>>(std::forward<Function>(f))} {}

  ~shared_packaged_task() = default;

  // copy
  shared_packaged_task(const shared_packaged_task&) = default;
  shared_packaged_task& operator=(const shared_packaged_task&) = default;

  // move support
  shared_packaged_task(shared_packaged_task&&) noexcept = default;
  shared_packaged_task& operator=(shared_packaged_task&&) noexcept = default;

  std::future<result_type> get_future() { return packaged_task_->get_future(); }

  void operator()(Args&&... args) { packaged_task_->operator()(std::forward<Args>(args)...); }

 private:
  std::shared_ptr<std::packaged_task<R(Args...)>> packaged_task_;
};

}  // namespace xlib

#endif  // XLIB_THREAD_FUTURE_SHAREDPACKAGETASK_HPP_
