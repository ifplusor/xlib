#ifndef XLIB_TYPES_TYPETRAITS_DECAY_HPP_
#define XLIB_TYPES_TYPETRAITS_DECAY_HPP_

#include <type_traits>

#include "xlib/__config.hpp"
#include "xlib/types/type_traits/conditional.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"

namespace xlib {

using std::decay;

#if _XLIB_STD_VER >= 2014L

using std::decay_t;

#else  // _XLIB_STD_VER >= 2014L

template <typename T>
using decay_t = typename std::decay<T>::type;

#endif  // _XLIB_STD_VER >= 2014L

/**
 * @brief decay type `T`, only if `T` is not lvalue reference.
 *
 * @tparam T
 *
 * @note reserve constant reference
 */
template <typename T>
using decay_rvalue_t = conditional_t<std::is_lvalue_reference_v<T>, T, remove_cvref_t<T>>;

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_DECAY_HPP_
