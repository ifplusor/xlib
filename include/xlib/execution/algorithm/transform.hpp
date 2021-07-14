#ifndef XLIB_EXECUTION_ALGORITHM_TRANSFORM_HPP_
#define XLIB_EXECUTION_ALGORITHM_TRANSFORM_HPP_

#include <exception>  // std::current_exception
#include <utility>    // std::forward, std::move

#include "xlib/__config.hpp"
#include "xlib/concepts/invocable.hpp"
#include "xlib/execution/concept/receiver.hpp"
#include "xlib/execution/concept/sender.hpp"
#include "xlib/execution/concept/sender_to.hpp"
#include "xlib/execution/receiver/set_done.hpp"
#include "xlib/execution/receiver/set_error.hpp"
#include "xlib/execution/receiver/set_value.hpp"
#include "xlib/execution/sender/connect.hpp"
#include "xlib/execution/type_traits/connect_traits.hpp"
#include "xlib/execution/type_traits/sender_traits.hpp"
#include "xlib/execution/type_traits/sender_value.hpp"
#include "xlib/meta/type_list.hpp"
#include "xlib/meta/type_set.hpp"
#include "xlib/types/type_traits/apply_result.hpp"
#include "xlib/types/type_traits/is_constructible.hpp"
#include "xlib/utility/functional/invoke.hpp"

namespace xlib::execution {

namespace detail {

template <typename Receiver, typename Function>
struct __transform_receiver {
  Function function;
  Receiver receiver;

  template <typename... Values>
  void set_value(Values&&... values) noexcept {
    using result_type = invoke_result_t<Function, Values...>;
    if constexpr (is_void_v<result_type>) {
      if constexpr (noexcept(invoke(std::move(function), std::forward<Values>(values)...))) {
        invoke(std::move(function), std::forward<Values>(values)...);
        ::xlib::execution::set_value(std::move(receiver));
      } else {
        try {
          invoke(std::move(function), std::forward<Values>(values)...);
          ::xlib::execution::set_value(receiver);
        } catch (...) {
          ::xlib::execution::set_error(std::move(receiver), std::current_exception());
        }
      }
    } else {
      if constexpr (noexcept(invoke(std::move(function), std::forward<Values>(values)...))) {
        ::xlib::execution::set_value(std::move(receiver), invoke(std::move(function), std::forward<Values>(values)...));
      } else {
        try {
          ::xlib::execution::set_value(receiver, invoke(std::move(function), std::forward<Values>(values)...));
        } catch (...) {
          ::xlib::execution::set_error(std::move(receiver), std::current_exception());
        }
      }
    }
  }

  template <typename Error>
  void set_error(Error&& error) noexcept {
    ::xlib::execution::set_error(std::move(receiver), std::forward<Error>(error));
  }

  void set_done() noexcept { ::xlib::execution::set_done(std::move(receiver)); }
};

template <typename Predecessor, typename Function>
struct __transform_sender {
  Predecessor predecessor;
  Function function;

  template <template <typename...> class Variant, template <typename...> class Tuple>
  using value_types = Variant<
      typename apply_result_t<Function, typename sender_value_traits<Predecessor>::value_types>::template apply<Tuple>>;

  template <template <typename...> class Variant>
  using error_types = typename sender_traits<Predecessor>::template error_types<type_set>::template insert<
      std::exception_ptr>::list_type::template apply<Variant>;

  static constexpr bool sends_done = sender_traits<Predecessor>::sends_done;

  // clang-format off
  xlib_template (typename Receiver)
  (requires receiver<Receiver> && sender_to<Predecessor, __transform_receiver<Receiver, Function>>)
  auto connect(Receiver&& receiver) noexcept(
      is_nothrow_connectable_v<Predecessor, __transform_receiver<Receiver, Function>>)
      -> connect_result_t<Predecessor, __transform_receiver<Receiver, Function>> {
    return ::xlib::execution::connect(
        std::move(predecessor),
        __transform_receiver<Receiver, Function>{std::move(function), std::forward<Receiver>(receiver)});
  }
  // clang-format on
};

/**
 * @brief The name `execution::transform` denotes a customization point object.
 * @details
 * For some subexpressions `s` and `f`, let `S` be a type such that `decltype((s))` is `S` and `decltype((f))` is `F`.
 * The expression `execution::transform(s, f)` is expression-equivalent to:
 */
struct __transform_function {
  /**
   * @details
   * - `s.transform(f)` if that expression is valid, `s` satisfies `sender`.
   *
   * - Otherwise, `transform(S, F)`, if that expression is valid, `s` satisfies `sender` with overload resolution
   *   performed in a context that includes the declaration `void transform() = delete;` and that does not include a
   *   declaration of `execution::transform`.
   */

  /**
   * @details
   * - Otherwise constructs a `receiver`, `r` and passes that receiver to `execution::connect(s, r)` to return an
   *   `operation_state os` such that:
   *
   *     When some `output_receiver` has been passed to `connect` on the returned `sender` to return some
   *     `operation_state os2`:
   *
   *     - If `set_value(r, Ts... ts)` is called, calls `std::invoke(F, ts...)` and passes the result `v` to
   *       `execution::set_value(output_receiver, v)`.
   *
   *     - If `f` throws, catches the exception and passes it to `execution::set_error(output_receiver, e)`.
   *
   *     - If `set_error(r, e)` is called, passes e to `execution::set_error(output_receiver, e)`.
   *
   *     - If `set_done(r)` is called, calls `execution::set_done(output_receiver)`.
   *
   *     When `start()` is called on `os2` calls `execution::start(os)`.
   */
  // clang-format off
  xlib_template (typename Sender, typename Function)
  (requires sender<Sender> /* && invocable<Function> */)
  auto operator()(Sender&& sender, Function&& function) const
      noexcept(is_nothrow_constructible_v<__transform_sender<Sender, Function>, Sender, Function>)
          -> __transform_sender<Sender, Function> {
    return {std::forward<Sender>(sender), std::forward<Function>(function)};
  }
  // clang-format on

  /**
   * - Otherwise the expression `execution::transform(s, f)` is ill-formed.
   */
};

template <typename T = __transform_function>
_XLIB_INLINE_VAR constexpr T __transform_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief
 * `transform` is a sender adapter that takes a `sender` and an `invocable` and returns a `sender` that propagates the
 * value resulting from calling the invocable on the value passed by the preceding `sender`.
 */
_XLIB_INLINE_VAR constexpr const auto& transform = detail::__transform_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_ALGORITHM_TRANSFORM_HPP_
