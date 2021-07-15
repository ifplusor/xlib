#ifndef XLIB_TYPES_TYPETRAITS_ADDREFERENCE_HPP_
#define XLIB_TYPES_TYPETRAITS_ADDREFERENCE_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::add_lvalue_reference;
using std::add_rvalue_reference;

#if _XLIB_STD_VER >= 2014L

using std::add_lvalue_reference_t;
using std::add_rvalue_reference_t;

#else  // _XLIB_STD_VER >= 2014L

template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

#endif  // _XLIB_STD_VER >= 2014L

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_REMOVEREFERENCE_HPP_
