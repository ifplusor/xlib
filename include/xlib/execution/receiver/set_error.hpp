#ifndef XLIB_EXECUTION_RECEIVER_SETERROR_HPP_
#define XLIB_EXECUTION_RECEIVER_SETERROR_HPP_

#include <exception>

#include "xlib/__config.hpp"

namespace xlib::execution {

namespace detail {

/**
 * @brief The `name execution::set_error` denotes a customization point object.
 * @details
 * The expression `execution::set_error(R, E)` for some subexpressions R and E are expression-equivalent to:
 */
struct __set_error_function {
  /**
   * @details
   * - `R.set_error(E)`, if that expression is valid. If the function selected does not send the error `E` to the
   *   receiver `R` ’s error channel, the program is ill-formed with no diagnostic required.
   *
   * @tparam Receiver
   * @param receiver
   * @param exception
   */
  template <typename Receiver, typename Error = std::exception_ptr>
  void operator()(Receiver&& receiver, Error&& error) const
      noexcept(noexcept(std::declval<Receiver>().set_error(std::declval<Error>()))) {
    std::forward<Receiver>(receiver).set_error(std::forward<Error>(error));
  }

  /**
   * - Otherwise, `set_error(R, E)`, if that expression is valid, with overload resolution performed in a context that
   *   includes the declaration `void set_error();` and that does not include a declaration of `execution::set_error`.
   *   If the function selected by overload resolution does not send the error E to the receiver R’s error channel, the
   *   program is ill-formed with no diagnostic required.
   *
   * - Otherwise, `execution::set_error(R, E)` is ill-formed.
   *
   */
};

template <typename T = __set_error_function>
_XLIB_INLINE_VAR constexpr T __set_error_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief signal error (ep is `std::exception_ptr`), never fails
 */
_XLIB_INLINE_VAR constexpr const auto& set_error = detail::__set_error_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_RECEIVER_SETERROR_HPP_
