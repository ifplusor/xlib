#ifndef XLIB_EXECUTION_ALGORITHM_ON_CPP_
#define XLIB_EXECUTION_ALGORITHM_ON_CPP_

#include <utility>

#include "xlib/__config.hpp"
#include "xlib/execution/algorithm/cooperate.hpp"
#include "xlib/execution/concept/scheduler.hpp"
#include "xlib/execution/concept/sender.hpp"
#include "xlib/execution/scheduler/schedule.hpp"

namespace xlib::execution {

namespace detail {

/**
 * @brief The name `execution::on` denotes a customization point object.
 * @details
 * For some subexpressions `s` and `sch`, let `S` be a type such that `decltype((s))` is `S` and `Sch` be a type such
 * that `decltype((sch))` is `Sch` The expression `execution::on(s, sch)` is expression-equivalent to:
 */
struct __on_function {
  /**
   * @details
   * - `s.on(sch)` if that expression is valid, if `S` satisfies `sender`
   *
   * - Otherwise, `on(s, sch)` if that expression is valid, and if `S` satisfies `sender` and if `Sch` satisfies
   *   `scheduler`, with overload resolution performed in a context that includes the declaration `void on() = delete;`
   *   and that does not include a declaration of `execution::on`.
   */

  /**
   * @details
   * - Otherwise, `cooperate(s, schedule(sch))`
   *
   * @see `execution::cooperate`
   */
  // clang-format off
  xlib_template (typename Sender, typename Scheduler)
  (requires sender<Sender> && scheduler<Scheduler>)
  auto operator()(Sender&& sender, Scheduler&& scheduler) const {
    auto schedule_sender = ::xlib::execution::schedule(std::forward<Scheduler>(scheduler));
    return ::xlib::execution::cooperate(std::forward<Sender>(sender), std::move(schedule_sender));
  }
  // clang-format on

  /**
   * @details
   * - Otherwise, `execution::on(s, sch)` is ill-formed.
   *
   * Any customisation of `execution::on(s, sch)` returning a `sender s2` shall execute calls to `set_value`, `set_done`
   * or `set_error` on a `receiver` connected to `s2` on an execution context owned by `sch`.
   */
};

template <typename T = __on_function>
_XLIB_INLINE_VAR constexpr T __on_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief
 * Takes a `sender` and a `scheduler` and ensures that the `sender` operation is `connect` ed and `start` ed on the
 * execution context associated with the `scheduler`, giving the programmer control over where the work encapsulated by
 * `sender` is started.
 */
_XLIB_INLINE_VAR constexpr const auto& on = detail::__on_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_ALGORITHM_ON_CPP_
