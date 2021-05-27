#ifndef XLIB_EXECUTION_SENDER_CONNECT_HPP_
#define XLIB_EXECUTION_SENDER_CONNECT_HPP_

#include <utility>  // std::forward

#include "xlib/__config.hpp"
#include "xlib/concepts/concept.hpp"
#include "xlib/execution/concept/has_connect.hpp"
#include "xlib/execution/concept/receiver.hpp"
#include "xlib/execution/concept/sender.hpp"

namespace xlib::execution {

namespace detail {

template <typename Sender, typename Receiver>
void connect(Sender&&, Receiver&&) = delete;

/**
 * @details
 * For some subexpressions s and r, let S be `decltype((s))` and let R be `decltype((r))`. If R does not satisfy
 * receiver, `execution::connect(s, r)` is ill-formed; otherwise, the expression `execution::connect(s, r)` is
 * expression-equivalent to:
 */
struct __connect_function {
  /**
   * @details
   * - `s.connect(r)`, if that expression is valid, if its type satisfies operation_state, and if S satisfies sender.
   *
   * @tparam Sender
   * @tparam Receiver
   * @param sender
   * @param receiver
   * @return member_connect_result_t<Sender, Receiver>
   */
  // clang-format off
  xlib_template (typename Sender, typename Receiver)
  (requires sender<Sender> && receiver<Receiver> && __has_member_connect<Sender, Receiver>)
  member_connect_result_t<Sender, Receiver> operator()(Sender&& sender, Receiver&& receiver) const
      noexcept(detail::__has_nothrow_member_connect<Sender, Receiver>) {
    return std::forward<Sender>(sender).connect(std::forward<Receiver>(receiver));
  }
  // clang-format on

  // TODO
  /**
   * - Otherwise, `connect(s, r)`, if that expression is valid, if its type satisfies operation_state, and if S
   *   satisfies sender, with overload resolution performed in a context that includes the declaration `void connect();`
   *   and that does not include a declaration of execution::connect.
   *
   * - Otherwise, `execution::connect(s, r)` is ill-formed.
   */
};

template <typename T = __connect_function>
_XLIB_INLINE_VAR constexpr T __connect_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief The name `execution::connect` denotes a customization point object.
 */
_XLIB_INLINE_VAR constexpr const auto& connect = detail::__connect_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_SENDER_CONNECT_HPP_
