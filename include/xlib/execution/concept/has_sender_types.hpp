#ifndef XLIB_EXECUTION_CONCEPT_HASSENEDERTYPES_HPP_
#define XLIB_EXECUTION_CONCEPT_HASSENEDERTYPES_HPP_

#include "xlib/__config.hpp"

#if defined(_XLIB_VARIABLE_TEMPLATES)

#include "xlib/concepts/concept.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"

namespace xlib::execution::detail {

template <template <template <typename...> typename Variant, template <typename...> typename Tuple> typename>
struct has_value_types;  // exposition only

template <template <template <typename...> typename Variant> typename>
struct has_error_types;  // exposition only

// clang-format off
xlib_template (typename S)(
concept (__has_sender_types_impl)(S),
  xlib_concept_typename(has_value_types<S::template value_types>) xlib_concept_and
  xlib_concept_typename(has_error_types<S::template error_types>) xlib_concept_and
  xlib_concept_typename(bool_constant<S::sends_done>)
);
// clang-format on

template <typename S>
xlib_concept __has_sender_types = xlib_concept_ref(__has_sender_types_impl, S);

}  // namespace xlib::execution::detail

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

#endif  // XLIB_EXECUTION_CONCEPT_HASSENEDERTYPES_HPP_
