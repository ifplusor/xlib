#ifndef XLIB_EXECUTION_SCHEDULER_SCHEDULE_HPP_
#define XLIB_EXECUTION_SCHEDULER_SCHEDULE_HPP_

#include <utility>  // std::forward

#include "xlib/__config.hpp"
#include "xlib/concepts/concept.hpp"
#include "xlib/execution/concept/has_schedule.hpp"

namespace xlib::execution {

namespace detail {

template <typename Scheduler>
void schedule(Scheduler&&) = delete;

/**
 * @details
 * For some subexpression s, let S be `decltype((s))`. The expression `execution::schedule(s)` is expression-equivalent
 * to:
 */
struct __schedule_function {
  /**
   * @details
   * - `s.schedule()`, if that expression is valid and its type models sender.
   *
   * @tparam Scheduler
   * @param scheduler
   * @return schedule_member_result_t<Scheduler>
   */
  // clang-format off
  xlib_template (typename Scheduler)
  (requires __has_member_schedule<Scheduler>)
  member_schedule_result_t<Scheduler> operator()(Scheduler&& scheduler) const {
    return std::forward<Scheduler>(scheduler).schedule();
  }
  // clang-format on

  // TODO
  /**
   * - Otherwise, `schedule(s)`, if that expression is valid and its type models sender with overload resolution
   *   performed in a context that includes the declaration `void schedule();` and that does not include a declaration
   *   of `execution::schedule`.
   *
   * - Otherwise, execution::schedule(s) is ill-formed.
   */
};

template <typename T = __schedule_function>
_XLIB_INLINE_VAR constexpr T __schedule_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief The name `execution::schedule` denotes a customization point object.
 */
_XLIB_INLINE_VAR constexpr const auto& schedule = detail::__schedule_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_SCHEDULER_SCHEDULE_HPP_
