#ifndef XLIB_TYPES_TYPETRAITS_ISFUNCTION_HPP_
#define XLIB_TYPES_TYPETRAITS_ISFUNCTION_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::is_function;

#if _XLIB_STD_VER >= 2017L

using std::is_function_v;

#elif defined(_XLIB_VARIABLE_TEMPLATES)

template <typename T>
_XLIB_INLINE_VAR constexpr bool is_function_v = std::is_function<T>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_ISFUNCTION_HPP_
