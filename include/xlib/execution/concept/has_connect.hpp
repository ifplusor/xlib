#ifndef XLIB_EXECUTION_CONCEPT_HASCONNECT_HPP_
#define XLIB_EXECUTION_CONCEPT_HASCONNECT_HPP_

#include <utility>  // std::declval, std::forward

#include "xlib/concepts/concept.hpp"
#include "xlib/execution/concept/operation_state.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib::execution::detail {

#if defined(_XLIB_CONCEPTS)

template <typename Sender, typename Receiver>
xlib_concept __has_member_connect = requires(Sender&& sender, Receiver&& receiver) {
  { std::forward<Sender>(sender).connect(std::forward<Receiver>(receiver)) } -> operation_state;
};

template <typename Sender, typename Receiver>
xlib_concept __has_nothrow_member_connect = requires(Sender&& sender, Receiver&& receiver) {
  { std::forward<Sender>(sender).connect(std::forward<Receiver>(receiver)) } noexcept -> operation_state;
};

#else  // defined(_XLIB_CONCEPTS)

template <typename Sender, typename Receiver, typename = void>
struct __member_connect_traits {
  static constexpr bool is_valid = false;
  static constexpr bool is_noexcept = false;
};

template <typename Sender, typename Receiver>
struct __member_connect_traits<Sender,
                               Receiver,
                               void_t<decltype(std::declval<Sender>().connect(std::declval<Receiver>()))>> {
  using result_type = decltype(std::declval<Sender>().connect(std::declval<Receiver>()));

  static constexpr bool is_valid = operation_state<result_type>;
  static constexpr bool is_noexcept = is_valid && noexcept(std::declval<Sender>().connect(std::declval<Receiver>()));
};

template <typename Sender, typename Receiver>
xlib_concept __has_member_connect = __member_connect_traits<Sender, Receiver>::is_valid;

template <typename Sender, typename Receiver>
xlib_concept __has_nothrow_member_connect = __member_connect_traits<Sender, Receiver>::is_noexcept;

#endif  // defined(_XLIB_CONCEPTS)

template <typename Sender, typename Receiver>
using member_connect_result_t = decltype(std::declval<Sender>().connect(std::declval<Receiver>()));

}  // namespace xlib::execution::detail

#endif  // XLIB_EXECUTION_TYPETRAITS_HASCONNECTTRAITS_HPP_
