#ifndef XLIB_TYPES_TYPETRAITS_ENABLEIF_HPP_
#define XLIB_TYPES_TYPETRAITS_ENABLEIF_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::enable_if;

#if _XLIB_STD_VER >= 2014L

using std::enable_if_t;

#else  // _XLIB_STD_VER >= 2014L

template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

#endif  // _XLIB_STD_VER >= 2014L

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_ENABLEIF_HPP_
