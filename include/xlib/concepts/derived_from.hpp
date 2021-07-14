#ifndef XLIB_CONCEPTS_DERIVEDFROM_HPP_
#define XLIB_CONCEPTS_DERIVEDFROM_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_LIB_CONCEPTS)

#include <concepts>

namespace xlib {

using std::derived_from;

}

#elif defined(_XLIB_VARIABLE_TEMPLATES)

#include "xlib/concepts/concept.hpp"
#include "xlib/types/type_traits/is_base_of.hpp"
#include "xlib/types/type_traits/is_convertible.hpp"

namespace xlib {

template <typename Derived, typename Base>
xlib_concept derived_from = xlib_is_base_of_v(Base, Derived) &&
                            xlib_is_convertible_v(const volatile Derived*, const volatile Base*);

}  // namespace xlib

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_CONCEPTS_DERIVEDFROM_HPP_
