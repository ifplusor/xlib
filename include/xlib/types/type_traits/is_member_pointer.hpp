#ifndef XLIB_TYPES_TYPETRAITS_ISMEMBERPOINTER_HPP_
#define XLIB_TYPES_TYPETRAITS_ISMEMBERPOINTER_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::is_member_pointer;

#if _XLIB_STD_VER >= 2017L

using std::is_member_pointer_v;

#elif defined(_XLIB_VARIABLE_TEMPLATES)

template <typename T>
_XLIB_INLINE_VAR constexpr bool is_member_pointer_v = std::is_member_pointer<T>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_ISMEMBERPOINTER_HPP_
