#ifndef XLIB_TYPES_TYPETRAITS_ISONEOF_HPP_
#define XLIB_TYPES_TYPETRAITS_ISONEOF_HPP_

#include "xlib/__config.hpp"
#include "xlib/types/type_traits/conditional.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"
#include "xlib/types/type_traits/is_same.hpp"

namespace xlib {

template <typename T, typename... Us>
struct is_one_of : conditional_t<(is_same<T, Us>::value || ...), true_type, false_type> {};

#if defined(_XLIB_VARIABLE_TEMPLATES)

template <typename T, typename... Us>
_XLIB_INLINE_VAR constexpr bool is_one_of_v = is_one_of<T, Us...>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_ISONEOF_HPP_
