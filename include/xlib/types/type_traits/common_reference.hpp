#ifndef XLIB_TYPES_TYPETRAITS_COMMONREFERENCE_HPP_
#define XLIB_TYPES_TYPETRAITS_COMMONREFERENCE_HPP_

#include <type_traits>

namespace xlib {

// TODO
template <typename... T>
struct common_reference;

template <typename... T>
using common_reference_t = typename common_reference<T...>::type;

}  // namespace xlib

#endif  // XLIB_TYPES_TYPETRAITS_COMMONREFERENCE_HPP_
