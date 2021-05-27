#ifndef XLIB_EXECUTION_TYPETRAITS_ISSENDER_HPP_
#define XLIB_EXECUTION_TYPETRAITS_ISSENDER_HPP_

#include "xlib/__config.hpp"
#include "xlib/execution/concept/sender.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"

namespace xlib::execution::detail {

template <typename Sender>
struct is_sender : std::conditional_t<sender<Sender>, true_type, false_type> {};

#if defined(_XLIB_INLINE_VARIABLES)

template <typename Sender>
_XLIB_INLINE_VAR constexpr bool is_sender_v = is_sender<Sender>::value;

#endif  // defined(_XLIB_INLINE_VARIABLES)

}  // namespace xlib::execution::detail

#endif  // XLIB_EXECUTION_TYPETRAITS_ISSENDER_HPP_
