#ifndef XLIB_TYPES_TYPETRAITS_ISSAME_HPP_
#define XLIB_TYPES_TYPETRAITS_ISSAME_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::is_same;

#if _XLIB_STD_VER >= 2017L

using std::is_same_v;

#elif defined(_XLIB_VARIABLE_TEMPLATES)

template <class T, class U>
_XLIB_INLINE_VAR constexpr bool is_same_v = is_same<T, U>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_ISSAME_HPP_
