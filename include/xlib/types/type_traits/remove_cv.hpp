#ifndef XLIB_TYPES_TYPETRAITS_REMOVECV_HPP_
#define XLIB_TYPES_TYPETRAITS_REMOVECV_HPP_

#include <type_traits>

namespace xlib {

using std::remove_cv;

#if _XLIB_STD_VER >= 2014L

using std::remove_cv_t;

#else  // _XLIB_STD_VER >= 2014L

template <class T>
using remove_cv_t = typename remove_cv<T>::type;

#endif  // _XLIB_STD_VER >= 2014L

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_REMOVECV_HPP_
