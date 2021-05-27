#ifndef XLIB_EXECUTION_SUPPORT_ASRECEIVER_HPP_
#define XLIB_EXECUTION_SUPPORT_ASRECEIVER_HPP_

#include <exception>  // std::terminate

#include "xlib/utility/functional/invoke.hpp"
#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/is_invocable.hpp"

namespace xlib::execution {

template <typename Function, typename Sender = void>
struct as_receiver {
  decay_t<Function> function_;

  template <typename... Values>
  void set_value(Values&&... values) noexcept(is_nothrow_invocable_v<Function&, Values&&...>) {
    invoke(function_, std::forward<Values>(values)...);
  }

  template <typename E>
  [[noreturn]] void set_error(E&& /*unused*/) noexcept {
    std::terminate();
  }

  void set_done() noexcept {}
};

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_SUPPORT_ASRECEIVER_HPP_
