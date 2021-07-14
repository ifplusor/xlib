#ifndef XLIB_CONCEPTS_DESTRUCTIBLE_HPP_
#define XLIB_CONCEPTS_DESTRUCTIBLE_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_LIB_CONCEPTS)

#include <concepts>

namespace xlib {

using std::destructible;

}

#elif defined(_XLIB_VARIABLE_TEMPLATES)

#include "xlib/concepts/concept.hpp"
#include "xlib/types/type_traits/is_destructible.hpp"

namespace xlib {

template <typename T>
xlib_concept destructible = xlib_is_nothrow_destructible_v(T);

}  // namespace xlib

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_CONCEPTS_DESTRUCTIBLE_HPP_
