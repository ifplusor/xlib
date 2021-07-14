#ifndef XLIB_EXECUTION_ALGORITHM_SYNCWAIT_HPP_
#define XLIB_EXECUTION_ALGORITHM_SYNCWAIT_HPP_

#include <exception>  // std::terminate

#include "xlib/__config.hpp"
#include "xlib/execution/concept/typed_sender.hpp"
#include "xlib/execution/context/task_queue.hpp"
#include "xlib/execution/operation_state/start.hpp"
#include "xlib/execution/sender/connect.hpp"
#include "xlib/execution/type_traits/sender_value.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"
#include "xlib/utility/result_state/result_state.hpp"

namespace xlib::execution {

namespace detail {

/**
 * @details
 * - If the operation completes by calling `set_value(r, t)` then `sync_wait()` will return a value, `x`, of type
 *   `remove_cvref_t<decltype(t)>`.
 *
 * - If the operation completes by calling `set_value(r)` then `sync_wait()` will return `void`.
 *
 * - If the operation completes by calling `set_error(r, e)` then `sync_wait()` calls `std::rethrow_exception(e)` if
 *   `decltype(e)` is `std::exception_ptr` or throw `e`; otherwise.
 *
 * - If the operation completes by calling `set_done(r)` then `sync_wait()` will call `std::terminate`.
 */
template <typename Result>
struct __sync_wait_receiver {
  result_state<Result>& state;
  task_queue& context;

  template <typename... Values>
  void set_value(Values&&... values) noexcept {
    try {
      state.set_result(std::forward<Values>(values)...);
    } catch (...) {
      state.set_exception(std::current_exception());
    }
    signal_complete();
  }

  void set_error(std::exception_ptr exception) noexcept {
    state.set_exception(exception);
    signal_complete();
  }

  // void set_error(std::error_code ec) && noexcept {
  //  set_error(std::make_exception_ptr(std::system_error{ec, "sync_wait"}));
  //}

  void set_done() noexcept { signal_complete(); }

 private:
  void signal_complete() noexcept { context.stop(); }
};

/**
 * @brief The name `execution::sync_wait` denotes a customization point object.
 * @details
 * For some subexpression `s` let `S` be a type such that `decltype((s))` is `S`. The expression
 * `execution::sync_wait(s)` is expression-equivalent to:
 */
struct __sync_wait_function {
  /**
   * @details
   * - `s.sync_wait()` if that expression is valid and if `S` satisfies `sender`.
   */

  /**
   * @details
   * - Otherwise, `sync_wait(s)`, if that expression is valid, if `S` satisfies `sender`, with overload resolution
   *   performed in a context that includes the declaration `void sync_wait() = delete;` and that does not include a
   *   declaration of `execution::sync_wait`.
   */

  /**
   * @details
   * - Otherwise constructs a `receiver`, `r` over an implementation-defined synchronization primitive and passes `r` to
   *   `execution::connect(s, r)` returning some `operation_state os`. Waits on the synchronization primitive to block
   *   on completion of `s`.
   *
   * @tparam Sender
   * @param sender
   * @return sender_single_value_result_t<remove_cvref_t<Sender>>
   */
  template <typed_sender Sender>
  auto operator()(Sender&& sender) const -> sender_single_value_result_t<remove_cvref_t<Sender>> {
    using Result = sender_single_value_result_t<remove_cvref_t<Sender>>;

    result_state<Result> state;
    task_queue context;

    auto operation =
        ::xlib::execution::connect(std::forward<Sender>(sender), __sync_wait_receiver<Result>{state, context});
    ::xlib::execution::start(operation);

    context.run_forever();

    if (!state.has_result()) {
      std::terminate();
    }
    return std::move(state.get());
  }
};

template <typename T = __sync_wait_function>
_XLIB_INLINE_VAR constexpr T __sync_wait_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief
 * Blocks the calling thread to wait for the passed `sender` to complete. Returns `T` when passed a `typed_sender` that
 * sends a `T` on the value channel, where `T` may be `void`, throws if an `exception` is propagated and calls
 * `std::terminate` on propagation of the `set_done()` signal.
 */
_XLIB_INLINE_VAR constexpr const auto& sync_wait = detail::__sync_wait_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_ALGORITHM_SYNCWAIT_HPP_
