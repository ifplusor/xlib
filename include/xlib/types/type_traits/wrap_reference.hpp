#ifndef XLIB_TYPES_TYPETRAITS_WRAPREFERENCE_HPP_
#define XLIB_TYPES_TYPETRAITS_WRAPREFERENCE_HPP_

#include <functional>  // std::reference_wrapper

#include "xlib/types/type_traits/conditional.hpp"
#include "xlib/types/type_traits/is_reference.hpp"
#include "xlib/types/type_traits/remove_reference.hpp"

namespace xlib {

/**
 * @brief wrap type `T`, only if `T` is reference.
 * 
 * @tparam T 
 */
template <typename T>
using wrap_reference_t = conditional_t<is_reference_v<T>, std::reference_wrapper<remove_reference_t<T>>, T>;

}  // namespace xlib

#endif  //  XLIB_TYPES_TYPETRAITS_WRAPREFERENCE_HPP_
