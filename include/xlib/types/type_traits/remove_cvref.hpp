#ifndef XLIB_TYPES_TYPETRAITS_REMOVECVREF_HPP_
#define XLIB_TYPES_TYPETRAITS_REMOVECVREF_HPP_

#include "xlib/__config.hpp"

#if _XLIB_STD_VER >= 2020L

#include <type_traits>

namespace xlib {

using std::remove_cvref;
using std::remove_cvref_t;

}  // namespace xlib

#else  // _XLIB_STD_VER >= 2020L

#include "xlib/types/type_traits/remove_cv.hpp"
#include "xlib/types/type_traits/remove_reference.hpp"

namespace xlib {

template <class T>
struct remove_cvref {
  using type = remove_cv_t<remove_reference_t<T>>;
};

template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;

}  // namespace xlib

#endif  // _XLIB_STD_VER >= 2020L

#endif  // XLIB_TYPES_TYPETRAITS_REMOVECVREF_HPP_
