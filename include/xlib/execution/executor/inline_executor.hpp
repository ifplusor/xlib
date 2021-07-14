#ifndef XLIB_EXECUTION_EXECUTOR_INLINEEXECUTOR_HPP_
#define XLIB_EXECUTION_EXECUTOR_INLINEEXECUTOR_HPP_

#include "xlib/utility/functional/invoke.hpp"

namespace xlib::execution {

class inline_executor {
  template <class Function>
  void execute(Function&& function) const noexcept {
    std::invoke(std::forward<Function>(function));
  }

#if 0   // __cpp_impl_three_way_comparison >= 201907L
  auto operator<=>(const inline_executor&) const = default;
#else   // __cpp_impl_three_way_comparison >= 201907L
  constexpr bool operator==(const inline_executor& /*unused*/) const noexcept { return true; }
  constexpr bool operator!=(const inline_executor& /*unused*/) const noexcept { return false; }
#endif  // __cpp_impl_three_way_comparison >= 201907L
};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_EXECUTOR_INLINEEXECUTOR_HPP_
