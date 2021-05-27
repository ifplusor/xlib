#ifndef XLIB_EXECUTION_ALGORITHM_JUST_HPP_
#define XLIB_EXECUTION_ALGORITHM_JUST_HPP_

#include <exception>  // std::current_exception
#include <tuple>      // std::tuple, std::apply
#include <utility>    // std::forward, std::move, std::in_place_t

#include "xlib/__config.hpp"
#include "xlib/concepts/concept.hpp"
#include "xlib/concepts/moveable_value.hpp"
#include "xlib/concepts/same_as.hpp"
#include "xlib/execution/concept/receiver.hpp"
#include "xlib/execution/receiver/set_error.hpp"
#include "xlib/execution/receiver/set_value.hpp"
#include "xlib/types/type_traits/decay.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"

namespace xlib::execution {

namespace detail {

template <typename Receiver, typename... Values>
struct __just_operation_impl {
  std::tuple<Values...> values;
  Receiver receiver;

  void start() & noexcept {
    try {
      std::apply([&](Values&&... values) { ::xlib::execution::set_value(receiver, std::forward<Values>(values)...); },
                 std::move(values));
    } catch (...) {
      ::xlib::execution::set_error(receiver, std::current_exception());
    }
  }
};

template <typename Receiver, typename... Values>
using __just_operation = __just_operation_impl<remove_cvref_t<Receiver>, Values...>;

template <typename... Values>
class __just_sender_impl {
 public:
  template <template <typename...> typename Variant, template <typename...> typename Tuple>
  using value_types = Variant<Tuple<Values...>>;

  template <template <typename...> typename Variant>
  using error_types = Variant<std::exception_ptr>;

  static constexpr bool sends_done = false;

  // clang-format off
  xlib_template (typename... Values2)
  (requires sizeof...(Values2) == sizeof...(Values) && constructible_from<std::tuple<Values...>, Values2...>)
  explicit __just_sender_impl(std::in_place_t /*unused*/, Values2&&... values) noexcept(
          is_nothrow_constructible_v<std::tuple<Values...>, Values2...>)
      : values_(std::forward<Values2>(values)...) {
  }
  // clang-format on

  // clang-format off
  xlib_template (typename Receiver)
  (requires receiver<Receiver> && constructible_from<std::tuple<Values...>, std::tuple<Values...>>)
  __just_operation<Receiver, Values...> connect(Receiver&& receiver) noexcept(
      is_nothrow_constructible_v<std::tuple<Values...>, decltype(std::declval<__just_sender_impl>().values_)>) {
    return {std::move(values_), std::forward<Receiver>(receiver)};
  }
  // clang-format on

  // static constexpr blocking_t query(blocking_t /*unused*/) { return ::xlib::execution::blocking_t::always; }

 private:
  std::tuple<Values...> values_;
};

template <typename... Values>
using __just_sender = __just_sender_impl<decay_t<Values>...>;

/**
 * @details
 * The expression `execution::just(t...)` returns a sender, `s` wrapping the values `t...`.
 */
struct __just_function {
  /**
   * @details
   *
   * - When `execution::connect(s, r)` is called resulting in `operation_state o` containing `rCopy` with type
   *   `remove_cvref_t<decltype(r)>` and initialized with `r` and followed by `execution::start(o)` for some `r`, will
   *   call `execution::set_value(r, std::move(t)...)`, inline with the caller.
   *
   * - If moving of `t` throws, then will catch the exception and call `execution::set_error(r, e)` with the caught
   *   `exception_ptr`.
   *
   * @tparam Values
   */
  // clang-format off
  xlib_template (typename... Values)
  (requires (...&& moveable_value<Values>))
  constexpr __just_sender<Values...> operator()(Values&&... values) const noexcept(
      is_nothrow_constructible_v<__just_sender<Values...>, std::in_place_t, Values...>) {
    return __just_sender<Values...>{std::in_place, std::forward<Values>(values)...};
  }
  // clang-format on
};

template <typename T = __just_function>
_XLIB_INLINE_VAR constexpr T __just_customization_point{};

}  // namespace detail

inline namespace cpos {

/**
 * @brief `just` creates a `sender` that propagates a value inline to a submitted receiver.
 */
_XLIB_INLINE_VAR constexpr const auto& just = detail::__just_customization_point<>;

}  // namespace cpos

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_ALGORITHM_JUST_HPP_
