#ifndef XLIB_EXECUTION_OPERATIONSTATE_START_HPP_
#define XLIB_EXECUTION_OPERATIONSTATE_START_HPP_

#include <utility>  // std::forward

#include "xlib/__config.hpp"
#include "xlib/concepts/concept.hpp"
#include "xlib/execution/concept/has_start.hpp"

namespace xlib::execution {

namespace detail {

template <typename OperationState>
void start(OperationState&&) = delete;

/**
 * @details
 * The expression `execution::start(O)` for some lvalue subexpression O is expression-equivalent to:
 */
struct __start_function {
  /**
   * @details
   * - `O.start()`, if that expression is valid.
   *
   * @tparam OperationState
   * @param operation_state
   * @return start_member_result_t<OperationState>
   */
  // clang-format off
  xlib_template (typename OperationState)
  (requires __has_member_start<OperationState>)
  member_start_result_t<OperationState> operator()(OperationState& operation_state) const noexcept {
    static_assert(__has_nothrow_member_start<OperationState>, "start() customization point must be noexcept");
    return operation_state.start();
  }
  // clang-format on

  // TODO
  /**
   * - Otherwise, `start(O)`, if that expression is valid, with overload resolution performed in a context that includes
   *   the declaration `void start();` and that does not include a declaration of `execution::start`.
   *
   * - Otherwise, `execution::start(O)` is ill-formed.
   */
};

template <typename T = __start_function>
_XLIB_INLINE_VAR constexpr T __start_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief The name `execution::start` denotes a customization point object.
 */
_XLIB_INLINE_VAR constexpr const auto& start = detail::__start_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_OPERATIONSTATE_START_HPP_
