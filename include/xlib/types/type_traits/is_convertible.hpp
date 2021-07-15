#ifndef XLIB_TYPES_TYPETRAITS_ISCONVERTIBLE_HPP_
#define XLIB_TYPES_TYPETRAITS_ISCONVERTIBLE_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::is_convertible;

#if _XLIB_STD_VER >= 2017L

using std::is_convertible_v;

#elif defined(_XLIB_VARIABLE_TEMPLATES)

template <class From, class To>
_XLIB_INLINE_VAR constexpr bool is_convertible_v = is_convertible<From, To>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#if _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)
#define xlib_is_convertible_v(...) ::xlib::is_convertible_v<__VA_ARGS__>
#else  // _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)
#define xlib_is_convertible_v(...) ::xlib::is_convertible<__VA_ARGS__>::value
#endif  // _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_TYPES_TYPETRAITS_ISCONVERTIBLE_HPP_
