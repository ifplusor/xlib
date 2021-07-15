#ifndef XLIB_TYPES_TYPETRAITS_INTEGRALCONSTANT_HPP_
#define XLIB_TYPES_TYPETRAITS_INTEGRALCONSTANT_HPP_

#include <type_traits>

#include "xlib/__config.hpp"

namespace xlib {

using std::integral_constant;

using std::false_type;
using std::true_type;

#if _XLIB_STD_VER >= 2017L

using std::bool_constant;

#else  // _XLIB_STD_VER >= 2017L

template <bool B>
using bool_constant = std::integral_constant<bool, B>;

#endif  // _XLIB_STD_VER >= 2017L

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_INTEGRALCONSTANT_HPP_
