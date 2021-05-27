#ifndef XLIB_EXECUTION_TYPETRAITS_SENDERTRAITS_HPP_
#define XLIB_EXECUTION_TYPETRAITS_SENDERTRAITS_HPP_

#include <exception>

#include "xlib/__config.hpp"
#include "xlib/concepts/derived_from.hpp"
#include "xlib/execution/concept/executor.hpp"
#include "xlib/execution/concept/has_sender_types.hpp"
#include "xlib/execution/sender/sender_base.hpp"
#include "xlib/execution/support/as_invocable.hpp"
#include "xlib/types/type_traits/conditional.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"

namespace xlib {
namespace execution {

namespace detail {

#if defined(_XLIB_LIB_CONCEPTS)
template <typename S>
struct __sender_traits_impl;
#endif  // defined(_XLIB_LIB_CONCEPTS)

// S has sender types
template <typename S>
#if defined(_XLIB_LIB_CONCEPTS)
requires __hash_sender_types<S>
struct __sender_traits_impl {
#else  // defined(_XLIB_LIB_CONCEPTS)
struct __sender_traits_impl0 {
#endif  // defined(_XLIB_LIB_CONCEPTS)
  template <template <typename...> typename Variant, template <typename...> typename Tuple>
  using value_types = typename S::template value_types<Variant, Tuple>;

  template <template <typename...> typename Variant>
  using error_types = typename S::template error_types<Variant>;

  static constexpr bool sends_done = S::sends_done;
};

struct void_receiver {  // exposition only
  void set_value() noexcept;
  void set_error(std::exception_ptr) noexcept;
  void set_done() noexcept;
};

// S is executor
template <typename S>
#if defined(_XLIB_LIB_CONCEPTS)
requires !__hash_sender_types<S> && __executor_of_impl<S, as_invocable<void_receiver, S>>
struct __sender_traits_impl {
#else  // defined(_XLIB_LIB_CONCEPTS)
struct __sender_traits_impl1 {
#endif  // defined(_XLIB_LIB_CONCEPTS)
  template <template <typename...> typename Variant, template <typename...> typename Tuple>
  using value_types = Variant<Tuple<>>;

  template <template <typename...> typename Variant>
  using error_types = Variant<std::exception_ptr>;

  static constexpr bool sends_done = true;
};

// S derived from sender_base
template <typename S>
#if defined(_XLIB_LIB_CONCEPTS)
requires !__hash_sender_types<S> && !__executor_of_impl<S, as_invocable<void_receiver, S>> && derived_from<S, sender_base>
struct __sender_traits_impl {
#else  // defined(_XLIB_LIB_CONCEPTS)
struct __sender_traits_impl2 {
#endif  // defined(_XLIB_LIB_CONCEPTS)
};

// no sender traits
template <typename S>
#if defined(_XLIB_LIB_CONCEPTS)
requires !__hash_sender_types<S> && !__executor_of_impl<S, as_invocable<void_receiver, S>> && !derived_from<S, sender_base>
struct __sender_traits_impl {
#else  // defined(_XLIB_LIB_CONCEPTS)
struct __sender_traits_impl3 {
#endif  // defined(_XLIB_LIB_CONCEPTS)
  using __unspecialized = void;  // exposition only
};

#if !defined(_XLIB_LIB_CONCEPTS)
template <typename S>
using __sender_traits_impl =
    conditional_t<__has_sender_types<S>, __sender_traits_impl0<S>,
    conditional_t<__executor_of_impl<S, as_invocable<void_receiver, S>>, __sender_traits_impl1<S>,
    conditional_t<derived_from<S, sender_base>, __sender_traits_impl2<S>, __sender_traits_impl3<S>>>>;
#endif  // !defined(_XLIB_LIB_CONCEPTS)

}  // namespace detail

template <typename Sender>
struct sender_traits : detail::__sender_traits_impl<Sender> {};

}  // namespace execution
}  // namespace xlib

#endif  // XLIB_EXECUTION_TYPETRAITS_SENDERTRAITS_HPP_
