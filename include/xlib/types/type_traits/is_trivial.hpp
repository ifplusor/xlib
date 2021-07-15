#ifndef XLIB_TYPES_TYPETRAITS_ISTRIVIAL_HPP_
#define XLIB_TYPES_TYPETRAITS_ISTRIVIAL_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::is_trivial;

#if _XLIB_STD_VER >= 2017L

using std::is_trivial_v;

#elif defined(_XLIB_VARIABLE_TEMPLATES)

template <class T>
_XLIB_INLINE_VAR constexpr bool is_trivial_v = is_trivial<T>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_ISTRIVIAL_HPP_
