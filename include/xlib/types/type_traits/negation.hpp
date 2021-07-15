#ifndef XLIB_TYPES_TYPETRAITS_NEGATION_HPP_
#define XLIB_TYPES_TYPETRAITS_NEGATION_HPP_

#include "xlib/__config.hpp"

#if _XLIB_STD_VER >= 2017L

#include <type_traits>

namespace xlib {

using std::negation;
using std::negation_v;

}  // namespace xlib

#else

#include "xlib/types/type_traits/integral_constant.hpp"

namespace xlib {

template <class B>
struct negation : std::bool_constant<!bool(B::value)> {};

#if defined(_XLIB_VARIABLE_TEMPLATES)

template <class B>
_XLIB_INLINE_VAR constexpr bool negation_v = negation<B>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // _XLIB_STD_VER >= 2017L

#endif  // XLIB_TYPES_TYPETRAITS_NEGATION_HPP_
