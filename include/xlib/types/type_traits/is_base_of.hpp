#ifndef XLIB_TYPES_TYPETRAITS_ISBASEOF_HPP_
#define XLIB_TYPES_TYPETRAITS_ISBASEOF_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::is_base_of;

#if _XLIB_STD_VER >= 2017L

using std::is_base_of_v;

#elif defined(_XLIB_VARIABLE_TEMPLATES)

template <typename Base, typename Derived>
_XLIB_INLINE_VAR constexpr bool is_base_of_v = std::is_base_of<Base, Derived>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

#if _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)
#define xlib_is_base_of_v(...) ::xlib::is_base_of_v<__VA_ARGS__>
#else  // _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)
#define xlib_is_base_of_v(...) ::xlib::is_base_of<__VA_ARGS__>::value
#endif  // _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_ISBASEOF_HPP_
