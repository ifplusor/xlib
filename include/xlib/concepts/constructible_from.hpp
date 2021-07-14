#ifndef XLIB_CONCEPTS_CONSTRUCTIBLEFROM_HPP_
#define XLIB_CONCEPTS_CONSTRUCTIBLEFROM_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_LIB_CONCEPTS)

#include <concepts>

namespace xlib {

using std::constructible_from;

}

#elif defined(_XLIB_VARIABLE_TEMPLATES)

#include "xlib/concepts/concept.hpp"
#include "xlib/concepts/destructible.hpp"
#include "xlib/types/type_traits/is_constructible.hpp"

namespace xlib {

template <class T, class... Args>
xlib_concept constructible_from = destructible<T> && xlib_is_constructible_v(T, Args...);

}  // namespace xlib

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_CONCEPTS_CONSTRUCTIBLEFROM_HPP_
