#ifndef XLIB_TYPES_TYPETRAITS_ISDESTRUCTIBLE_HPP_
#define XLIB_TYPES_TYPETRAITS_ISDESTRUCTIBLE_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::is_destructible;
using std::is_nothrow_destructible;
using std::is_trivially_destructible;

#if _XLIB_STD_VER >= 2017L

using std::is_destructible_v;
using std::is_nothrow_destructible_v;
using std::is_trivially_destructible_v;

#elif defined(_XLIB_VARIABLE_TEMPLATES)

template <class T, class... Args>
_XLIB_INLINE_VAR constexpr bool is_destructible_v = is_destructible<T, Args...>::value;

template <class T, class... Args>
_XLIB_INLINE_VAR constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T, Args...>::value;

template <class T, class... Args>
_XLIB_INLINE_VAR constexpr bool is_trivially_destructible_v = is_trivially_destructible<T, Args...>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#if _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)
#define xlib_is_destructible_v(...) xlib::is_destructible_v<__VA_ARGS__>
#define xlib_is_nothrow_destructible_v(...) xlib::is_nothrow_destructible_v<__VA_ARGS__>
#else  // _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)
#define xlib_is_destructible_v(...) xlib::is_destructible<__VA_ARGS__>::value
#define xlib_is_nothrow_destructible_v(...) xlib::is_nothrow_destructible<__VA_ARGS__>::value
#endif  // _XLIB_STD_VER >= 2017L || defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_TYPES_TYPETRAITS_ISDESTRUCTIBLE_HPP_
