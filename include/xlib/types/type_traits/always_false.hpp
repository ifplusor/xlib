#ifndef XLIB_TYPES_TYPETRAITS_ALWAYSFALSE_HPP_
#define XLIB_TYPES_TYPETRAITS_ALWAYSFALSE_HPP_

#include "xlib/__config.hpp"

namespace xlib {

template <typename T>
struct always_false {
  static constexpr bool value = false;
};

#if defined(_XLIB_VARIABLE_TEMPLATES)

template <typename T>
_XLIB_INLINE_VAR constexpr bool always_false_v = always_false<T>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_ALWAYSFALSE_HPP_
