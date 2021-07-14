#ifndef XLIB_EXECUTION_ASYNC_HPP_
#define XLIB_EXECUTION_ASYNC_HPP_

#include <future>
#include <utility>

#include "xlib/execution/executor/execute.hpp"
#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/invoke_result.hpp"

namespace xlib::execution {

template <typename Executor, typename Function, typename... Args>
auto async(const Executor& ex, Function&& func, Args&&... args)
#if __cplusplus < 201402L
    -> std::future<invoke_result_t<decay_t<Function>, decay_t<Args>...>>
#endif
{
  // package up the work
#if _XLIB_STD_VER >= 2017L
  std::packaged_task work(std::forward<Function>(func), std::forward<Args>(args)...);
#else
  using result_t = invoke_result_t<decay_t<Function>, decay_t<Args>...>;
  std::packaged_task<result_t(Args...)> work(std::forward<Function>(func), std::forward<Args>(args)...);
#endif

  // get the future
  auto future = work.get_future();

  // execute work on the given executor
  execute(ex, std::move(work));

  return future;
}

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_ASYNC_HPP_
