#ifndef XLIB_CONCEPTS_COMMONREFERENCEWITH_HPP_
#define XLIB_CONCEPTS_COMMONREFERENCEWITH_HPP_

#include "xlib/concepts/concept.hpp"

#if defined(_XLIB_LIB_CONCEPTS)

#include <concepts>

namespace xlib {

using std::common_reference_with;

}

#else  // defined(_XLIB_LIB_CONCEPTS)

#include "xlib/concepts/convertible_to.hpp"
#include "xlib/concepts/same_as.hpp"
#include "xlib/types/type_traits/common_reference.hpp"

namespace xlib {

template <class T, class U>
xlib_concept common_reference_with = same_as<common_reference_t<T, U>, common_reference_t<U, T>>
    && convertible_to<T, common_reference_t<T, U>>
    && convertible_to<U, common_reference_t<T, U>>;

}  // namespace xlib

#endif  // defined(_XLIB_LIB_CONCEPTS)

#endif  // XLIB_CONCEPTS_COMMONREFERENCEWITH_HPP_
