#ifndef XLIB_EXECUTION_EXECUTOR_EXECUTE_HPP_
#define XLIB_EXECUTION_EXECUTOR_EXECUTE_HPP_

#include <utility>

#include "xlib/__config.hpp"
#include "xlib/concepts/concept.hpp"
#include "xlib/concepts/invocable.hpp"
#include "xlib/execution/concept/has_execute.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"

namespace xlib::execution {

namespace detail {

/**
 * @details
 * For some subexpressions e and f, let E be `decltype((e))` and let F be `decltype((f))`. The expression
 * `execution::execute(e, f)` is ill-formed if F does not model invocable, or if E does not model executor. Otherwise,
 * it is expression-equivalent to:
 */
struct __execute_function {
  /**
   * @details
   * - `e.execute(f)`, if that expression is valid. If the function selected does not execute the function object f on
   *   the executor e, the program is ill-formed with no diagnostic required.
   *
   * @tparam Executor
   * @tparam Function
   * @param executor
   * @param function
   */
  // clang-format ovv
  xlib_template (typename Executor, typename Function)
  (requires invocable<Function> && __has_member_execute<Executor, Function>)
  void operator()(Executor&& executor, Function&& function) const {
    std::forward<Executor>(executor).execute(std::forward<Function>(function));
  }
  // clang-format on

  // TODO
  /**
   * - Otherwise, `execute(e, f)`, if that expression is valid, with overload resolution performed in a context that
   *   includes the declaration `void execute();` and that does not include a declaration of execution::execute. If the
   *   function selected by overload resolution does not execute the function object f on the executor e, the program is
   *   ill-formed with no diagnostic required.
   */
};

template <typename T = __execute_function>
_XLIB_INLINE_VAR constexpr T __execute_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief The name `execution::execute` denotes a customization point object.
 */
_XLIB_INLINE_VAR constexpr const auto& execute = detail::__execute_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_EXECUTOR_EXECUTE_HPP_