#ifndef XLIB_EXECUTION_RECEIVER_SETDONE_HPP_
#define XLIB_EXECUTION_RECEIVER_SETDONE_HPP_

#include <utility>  // std::forward, std::declval

#include "xlib/__config.hpp"

namespace xlib::execution {

namespace detail {

/**
 * @brief The name `execution::set_done` denotes a customization point object.
 * @details
 * The expression `execution::set_done(R)` for some subexpression R is expression-equivalent to:
 */
struct __set_done_function {
  /**
   * @details
   * - `R.set_done()`, if that expression is valid. If the function selected does not signal the receiver R’s done
   *   channel, the program is ill-formed with no diagnostic required.
   */
  template <typename Receiver>
  void operator()(Receiver&& receiver) const noexcept(noexcept(std::declval<Receiver>().set_done())) {
    std::forward<Receiver>(receiver).set_done();
  }

  // TODO
  /**
   *
   * - Otherwise, `set_done(R)`, if that expression is valid, with overload resolution performed in a context that
   *   includes the declaration `void set_done();` and that does not include a declaration of `execution::set_done`. If
   *   the function selected by overload resolution does not signal the receiver R’s done channel, the program is
   *   ill-formed with no diagnostic required.
   *
   * - Otherwise, `execution::set_done(R)` is ill-formed.
   */
};

template <typename T = __set_done_function>
_XLIB_INLINE_VAR constexpr T __set_done_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief signal stopped, never fails
 */
_XLIB_INLINE_VAR constexpr const auto& set_done = detail::__set_done_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_RECEIVER_SETDONE_HPP_
