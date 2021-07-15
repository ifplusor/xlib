#ifndef XLIB_TYPES_TYPETRAITS_XVALUEREFERENCE_HPP_
#define XLIB_TYPES_TYPETRAITS_XVALUEREFERENCE_HPP_

#include "xlib/types/type_traits/add_reference.hpp"
#include "xlib/types/type_traits/remove_reference.hpp"

namespace xlib {

template <typename T>
using xvalue_reference_t = add_rvalue_reference_t<remove_reference_t<T>>;

template <typename T>
inline constexpr xvalue_reference_t<T> cast_xvalue(T&& v) {
  return static_cast<xvalue_reference_t<T>>(v);
}

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_XVALUEREFERENCE_HPP_
