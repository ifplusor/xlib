#ifndef XLIB_EXECUTION_TYPETRAITS_CONNECTTRAITS_HPP_
#define XLIB_EXECUTION_TYPETRAITS_CONNECTTRAITS_HPP_

#include <utility>  // std::declval

#include "xlib/__config.hpp"
#include "xlib/execution/sender/connect.hpp"
#include "xlib/types/type_traits/conditional.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib::execution::detail {

template <typename Sender, typename Receiver, typename = void>
struct __connect_traits {
  static constexpr bool is_valid = false;
  static constexpr bool is_noexcept = false;
};

template <typename Sender, typename Receiver>
struct __connect_traits<
    Sender,
    Receiver,
    void_t<decltype(::xlib::execution::connect(std::declval<Sender>(), std::declval<Receiver>()))>> {
  using result_type = decltype(::xlib::execution::connect(std::declval<Sender>(), std::declval<Receiver>()));

  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept =
      noexcept(::xlib::execution::connect(std::declval<Sender>(), std::declval<Receiver>()));
};

template <typename Sender, typename Receiver>
struct is_connectable : conditional_t<__connect_traits<Sender, Receiver>::is_valid, true_type, false_type> {};

template <typename Sender, typename Receiver>
struct is_nothrow_connectable : conditional_t<__connect_traits<Sender, Receiver>::is_noexcept, true_type, false_type> {
};

#if defined(_XLIB_VARIABLE_TEMPLATES)

template <typename Sender, typename Receiver>
_XLIB_INLINE_VAR constexpr bool is_connectable_v = is_connectable<Sender, Receiver>::value;

template <typename Sender, typename Receiver>
_XLIB_INLINE_VAR constexpr bool is_nothrow_connectable_v = is_nothrow_connectable<Sender, Receiver>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

template <typename Sender, typename Receiver>
using connect_result_t = typename __connect_traits<Sender, Receiver>::result_type;

}  // namespace xlib::execution::detail

#endif  // XLIB_EXECUTION_TYPETRAITS_CONNECTTRAITS_HPP_
