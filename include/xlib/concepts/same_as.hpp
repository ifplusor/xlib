#ifndef XLIB_CONCEPTS_SAMEAS_HPP_
#define XLIB_CONCEPTS_SAMEAS_HPP_

#include "xlib/concepts/concept.hpp"

#if defined(_XLIB_LIB_CONCEPTS)

#include <concepts>

namespace xlib {

using std::same_as;

}

#elif defined(_XLIB_VARIABLE_TEMPLATES)

#include "xlib/types/type_traits/is_same.hpp"

namespace xlib {

namespace detail {

template <typename T, typename U>
xlib_concept __same_as_impl = is_same_v<T, U>;

}

template <typename T, typename U>
xlib_concept same_as = detail::__same_as_impl<T, U> && detail::__same_as_impl<U, T>;

}  // namespace xlib

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_CONCEPTS_SAMEAS_HPP_
