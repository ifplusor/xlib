#ifndef XLIB_TYPES_TYPETRAITS_ISCOPYCONSTRUCTIBLE_HPP_
#define XLIB_TYPES_TYPETRAITS_ISCOPYCONSTRUCTIBLE_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::is_copy_constructible;
using std::is_nothrow_copy_constructible;
using std::is_trivially_copy_constructible;

#if _XLIB_STD_VER >= 2017L

using std::is_copy_constructible_v;
using std::is_nothrow_copy_constructible_v;
using std::is_trivially_copy_constructible_v;

#elif defined(_XLIB_VARIABLE_TEMPLATES)

template <class T>
_XLIB_INLINE_VAR constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

template <class T>
_XLIB_INLINE_VAR constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;

template <class T>
_XLIB_INLINE_VAR constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_ISCOPYCONSTRUCTIBLE_HPP_
