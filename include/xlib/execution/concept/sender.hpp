#ifndef XLIB_EXECUTION_CONCEPT_SENEDER_HPP_
#define XLIB_EXECUTION_CONCEPT_SENEDER_HPP_

#include "xlib/concepts/concept.hpp"

#include "xlib/concepts/move_constructible.hpp"
#include "xlib/execution/type_traits/sender_traits.hpp"
#include "xlib/types/type_traits/remove_cvref.hpp"

namespace xlib::execution {

namespace detail {

// clang-format off
xlib_template (typename S)(
concept (__sender_helper)(S),
  xlib_concept_typename(typename sender_traits<remove_cvref_t<S>>::__unspecialized)
);
// clang-format on

}  // namespace detail

template <typename S>
xlib_concept sender = move_constructible<remove_cvref_t<S>> && !xlib_concept_ref(detail::__sender_helper, S);

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_CONCEPT_SENEDER_HPP_
