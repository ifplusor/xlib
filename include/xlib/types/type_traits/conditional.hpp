#ifndef XLIB_TYPES_TYPETRAITS_CONDITIONAL_HPP_
#define XLIB_TYPES_TYPETRAITS_CONDITIONAL_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::conditional;

#if _XLIB_STD_VER >= 2014L

using std::conditional_t;

#else  // _XLIB_STD_VER >= 2014L

template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;

#endif  // _XLIB_STD_VER >= 2014L

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_CONDITIONAL_HPP_
