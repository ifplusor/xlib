#ifndef XLIB_EXECUTION_SENDER_SUBMIT_HPP_
#define XLIB_EXECUTION_SENDER_SUBMIT_HPP_

#include <utility>  // std::forward

#include "xlib/__config.hpp"
#include "xlib/execution/concept/has_submit.hpp"
#include "xlib/execution/concept/receiver.hpp"
#include "xlib/execution/concept/sender.hpp"
#include "xlib/execution/concept/sender_to.hpp"
#include "xlib/execution/sender/connect.hpp"
// #include "xlib/execution/submit_state.hpp"

namespace xlib::execution {

namespace detail {

/**
 * @details
 * For some subexpressions s and r, let S be `decltype((s))` and let R be `decltype((r))`. The expression
 * `execution::submit(s, r)` is ill-formed if `sender_to<S, R>` is not true. Otherwise, it is expression-equivalent to:
 */
struct __submit_function {
  /**
   * @details
   * - `s.submit(r)`, if that expression is valid and S models sender. If the function selected does not submit the
   *   receiver object r via the sender s, the program is ill-formed with no diagnostic required.
   *
   * @tparam Sender
   * @tparam Receiver
   * @param sender
   * @param receiver
   * @return requires&&
   */
  template <sender Sender, receiver Receiver>
  requires sender_to<Sender, Receiver> && __has_member_submit<Sender, Receiver>
  auto operator()(Sender&& sender, Receiver&& receiver) {
    return std::forward<Sender>(sender).submit(std::forward<Receiver>(receiver));
  }

  // TODO
  /**
   * - Otherwise, `submit(s, r)`, if that expression is valid and S models sender, with overload resolution performed in
   * a context that includes the declaration `void submit();` and that does not include a declaration of
   *   `execution::submit`. If the function selected by overload resolution does not submit the receiver object r via
   * the sender s, the program is ill-formed with no diagnostic required.
   *
   * - Otherwise, `execution::start((newsubmit-state<S, R>{s,r})->state_)`, where submit-state is an
   *   implementation-defined class template.
   */

  // template <typename Sender, typename Receiver>
  // void operator()(Sender sender, Receiver receiver) {
  //  auto state = ::xlib::execution::connect(std::move(sender), std::move(receiver));
  //  ::xlib::execution::start(state);
  //}
};

template <class T = __submit_function>
_XLIB_INLINE_VAR constexpr T __submit_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief The name `execution::submit` denotes a customization point object.
 */
_XLIB_INLINE_VAR constexpr const auto& submit = detail::__submit_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_SENDER_SUBMIT_HPP_
