#ifndef XLIB_TYPES_TYPETRAITS_ISINTEGRAL_HPP_
#define XLIB_TYPES_TYPETRAITS_ISINTEGRAL_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::is_integral;

#if _XLIB_STD_VER >= 2017L

using std::is_integral_v;

#elif defined(_XLIB_VARIABLE_TEMPLATES)

template <class T>
_XLIB_INLINE_VAR constexpr bool is_integral_v = std::is_integral<T>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_ISINTEGRAL_HPP_
