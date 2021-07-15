#ifndef XLIB_TYPES_TYPETRAITS_REMOVEREFERENCE_HPP_
#define XLIB_TYPES_TYPETRAITS_REMOVEREFERENCE_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::remove_reference;

#if _XLIB_STD_VER >= 2014L

using std::remove_reference_t;

#else  // _XLIB_STD_VER >= 2014L

template <class T>
using remove_reference_t = typename remove_reference<T>::type;

#endif  // _XLIB_STD_VER >= 2014L

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_REMOVEREFERENCE_HPP_
