#ifndef XLIB_EXECUTION_RECEIVER_SETVALUE_HPP_
#define XLIB_EXECUTION_RECEIVER_SETVALUE_HPP_

#include <utility>  // std::declval, std::forward, std::move

#include "xlib/__config.hpp"

namespace xlib::execution {

namespace detail {

/**
 * @brief The name `execution::set_value` denotes a customization point object.
 * @details
 * The expression `execution::set_value(R, Vs...)` for some subexpressions R and Vs... is expression-equivalent to:
 */
struct __set_value_function {
  /**
   * @details
   * - `R.set_value(Vs...)`, if that expression is valid. If the function selected does not send the value(s) Vs... to
   *   the receiver R’s value channel, the program is ill-formed with no diagnostic required.
   *
   * @tparam Receiver
   * @tparam Values
   * @param receiver
   * @param values
   */
  template <typename Receiver, typename... Values>
  void operator()(Receiver&& receiver, Values&&... values) const
      noexcept(noexcept(std::declval<Receiver>().set_value(std::declval<Values>()...))) {
    std::forward<Receiver>(receiver).set_value(std::forward<Values>(values)...);
  }

  // TODO
  /**
   * - Otherwise, `set_value(R, Vs...)`, if that expression is valid, with overload resolution performed in a context
   *   that includes the declaration `void set_value();` and that does not include a declaration of
   *   `execution::set_value`. If the function selected by overload resolution does not send the value(s) Vs... to the
   *   receiver R’s value channel, the program is ill-formed with no diagnostic required.
   *
   * - Otherwise, `execution::set_value(R, Vs...)` is ill-formed.
   */
};

template <typename T = __set_value_function>
_XLIB_INLINE_VAR constexpr T __set_value_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief signal success, but `set_value` itself may fail
 */
_XLIB_INLINE_VAR constexpr const auto& set_value = detail::__set_value_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_RECEIVER_SETVALUE_HPP_
