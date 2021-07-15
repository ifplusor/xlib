#ifndef XLIB_TYPES_TYPETRAITS_ISCONSTRUCTIBLE_HPP_
#define XLIB_TYPES_TYPETRAITS_ISCONSTRUCTIBLE_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::is_constructible;
using std::is_nothrow_constructible;
using std::is_trivially_constructible;

#if _XLIB_STD_VER >= 2017L

using std::is_constructible_v;
using std::is_nothrow_constructible_v;
using std::is_trivially_constructible_v;

#elif defined(_XLIB_VARIABLE_TEMPLATES)

template <class T, class... Args>
_XLIB_INLINE_VAR constexpr bool is_constructible_v = is_constructible<T, Args...>::value;

template <class T, class... Args>
_XLIB_INLINE_VAR constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Args...>::value;

template <class T, class... Args>
_XLIB_INLINE_VAR constexpr bool is_trivially_constructible_v = is_trivially_constructible<T, Args...>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#if _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)
#define xlib_is_constructible_v(...) xlib::is_constructible_v<__VA_ARGS__>
#define xlib_is_nothrow_constructible_v(...) xlib::is_nothrow_constructible_v<__VA_ARGS__>
#else  // _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)
#define xlib_is_constructible_v(...) xlib::is_constructible<__VA_ARGS__>::value
#define xlib_is_nothrow_constructible_v(...) xlib::is_nothrow_constructible<__VA_ARGS__>::value
#endif  // _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_TYPES_TYPETRAITS_ISCONSTRUCTIBLE_HPP_
