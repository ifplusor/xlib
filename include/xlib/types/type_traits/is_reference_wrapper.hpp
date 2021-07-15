#ifndef XLIB_TYPES_TYPETRAITS_ISREFERENCEWRAPPER_HPP_
#define XLIB_TYPES_TYPETRAITS_ISREFERENCEWRAPPER_HPP_

#include <functional>  // std::reference_wrapper

#include "xlib/__config.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"

namespace xlib {

template <typename T>
struct is_reference_wrapper : false_type {};

template <typename U>
struct is_reference_wrapper<std::reference_wrapper<U>> : true_type {};

#if defined(_XLIB_VARIABLE_TEMPLATES)

template <typename T>
_XLIB_INLINE_VAR constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

}  // namespace xlib

#endif  //  XLIB_TYPES_TYPETRAITS_ISREFERENCEWRAPPER_HPP_
