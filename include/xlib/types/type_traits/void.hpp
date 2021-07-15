#ifndef XLIB_TYPES_TYPETRAITS_VOID_HPP_
#define XLIB_TYPES_TYPETRAITS_VOID_HPP_

#include "xlib/__config.hpp"

#if _XLIB_STD_VER >= 2017L

#include <type_traits>

namespace xlib {

using std::void_t;

}  // namespace xlib

#else  // _XLIB_STD_VER >= 2017L

namespace xlib {

template <class...>
using void_t = void;

}  // namespace xlib

#endif  // _XLIB_STD_VER >= 2017L

#endif  // XLIB_TYPES_TYPETRAITS_VOID_HPP_
