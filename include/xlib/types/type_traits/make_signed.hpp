#ifndef XLIB_TYPES_TYPETRAITS_MAKESIGNED_HPP_
#define XLIB_TYPES_TYPETRAITS_MAKESIGNED_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::make_signed;

#if _XLIB_STD_VER >= 2014L

using std::make_signed_t;

#else  // _XLIB_STD_VER >= 2014L

template <typename T>
using make_signed_t = typename std::make_signed<T>::type;

#endif  // _XLIB_STD_VER >= 2014L

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_MAKESIGNED_HPP_
