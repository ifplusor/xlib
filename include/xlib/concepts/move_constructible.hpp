#ifndef XLIB_CONCEPTS_MOVECONSTRUCTIBLE_HPP_
#define XLIB_CONCEPTS_MOVECONSTRUCTIBLE_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_LIB_CONCEPTS)

#include <concepts>

namespace xlib {

using std::move_constructible;

}

#elif defined(_XLIB_VARIABLE_TEMPLATES)

#include "xlib/concepts/concept.hpp"
#include "xlib/concepts/constructible_from.hpp"
#include "xlib/concepts/convertible_to.hpp"

namespace xlib {

template <typename T>
xlib_concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

}

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_CONCEPTS_MOVECONSTRUCTIBLE_HPP_
