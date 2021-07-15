#ifndef XLIB_TYPES_TYPETRAITS_ISREFERENCE_HPP_
#define XLIB_TYPES_TYPETRAITS_ISREFERENCE_HPP_

#include <type_traits>  // std::is_reference

#include "xlib/__config.hpp"

namespace xlib {

using std::is_reference;

#if _XLIB_STD_VER >= 2017L

using std::is_reference_v;

#elif defined(_XLIB_VARIABLE_TEMPLATES)

template <typename T>
_XLIB_INLINE_VAR constexpr bool is_reference_v = is_reference<T>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  //  XLIB_TYPES_TYPETRAITS_ISREFERENCE_HPP_
