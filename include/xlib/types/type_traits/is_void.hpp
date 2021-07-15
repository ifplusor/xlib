#ifndef XLIB_TYPES_TYPETRAITS_ISVOID_HPP_
#define XLIB_TYPES_TYPETRAITS_ISVOID_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::is_void;

#if _XLIB_STD_VER >= 2017L

using std::is_void_v;

#elif defined(_XLIB_VARIABLE_TEMPLATES)

template <class T>
_XLIB_INLINE_VAR constexpr bool is_void_v = is_void<T>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#if _XLIB_STD_VER >= 2017L && defined(_XLIB_VARIABLE_TEMPLATES)
#define xlib_is_void_v(...) ::xlib::is_void_v<__VA_ARGS__>
#else  // _XLIB_STD_VER >= 2017L && defined(_XLIB_VARIABLE_TEMPLATES)
#define xlib_is_void_v(...) ::xlib::is_void<__VA_ARGS__>::value
#endif  // _XLIB_STD_VER >= 2017L && defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_TYPES_TYPETRAITS_ISVOID_HPP_
