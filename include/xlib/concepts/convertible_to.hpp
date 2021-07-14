#ifndef XLIB_CONCEPTS_CONVERTIBLETO_HPP_
#define XLIB_CONCEPTS_CONVERTIBLETO_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_LIB_CONCEPTS)

#include <concepts>

namespace xlib {

using std::convertible_to;

}  // namespace xlib

#elif defined(_XLIB_VARIABLE_TEMPLATES)

#include "xlib/concepts/concept.hpp"
#include "xlib/types/type_traits/add_reference.hpp"
#include "xlib/types/type_traits/is_convertible.hpp"

namespace xlib {

template <typename From, typename To>
xlib_concept implicitly_convertible_to = xlib_is_convertible_v(From, To);

namespace detail {

// clang-format off
template <typename From, typename To>
xlib_requires(__explicitly_convertible_to_impl,
requires (add_rvalue_reference_t<From> (&from)()) (
  static_cast<To>(from())
));
// clang-format on

}  // namespace detail

template <typename From, typename To>
xlib_concept explicitly_convertible_to = xlib_requires_ref(detail::__explicitly_convertible_to_impl, From, To);

template <typename From, typename To>
xlib_concept convertible_to = implicitly_convertible_to<From, To> && explicitly_convertible_to<From, To>;

}  // namespace xlib

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_CONCEPTS_CONVERTIBLETO_HPP_
