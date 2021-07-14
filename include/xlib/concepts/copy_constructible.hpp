#ifndef XLIB_CONCEPTS_COPYCONSTRUCTIBLE_HPP_
#define XLIB_CONCEPTS_COPYCONSTRUCTIBLE_HPP_

#include "xlib/concepts/concept.hpp"

#if defined(_XLIB_LIB_CONCEPTS)

#include <concepts>

namespace xlib {

using std::copy_constructible;

}

#else  // defined(_XLIB_LIB_CONCEPTS)

#include "xlib/concepts/constructible_from.hpp"
#include "xlib/concepts/convertible_to.hpp"
#include "xlib/concepts/move_constructible.hpp"

namespace xlib {

template <typename T>
xlib_concept copy_constructible = move_constructible<T>
    && constructible_from<T, T&>
    && convertible_to<T&, T>
    && constructible_from<T, const T&>
    && convertible_to<const T&, T>
    && constructible_from<T, const T>
    && convertible_to<const T, T>;

}

#endif  // defined(_XLIB_LIB_CONCEPTS)

#endif  // XLIB_CONCEPTS_COPYCONSTRUCTIBLE_HPP_
