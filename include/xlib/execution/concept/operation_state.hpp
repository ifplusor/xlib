#ifndef XLIB_EXECUTION_CONCEPT_OPERATIONSTATE_HPP_
#define XLIB_EXECUTION_CONCEPT_OPERATIONSTATE_HPP_

#include "xlib/concepts/concept.hpp"

#include "xlib/concepts/destructible.hpp"
#include "xlib/execution/operation_state/start.hpp"
#include "xlib/types/type_traits/is_invocable.hpp"
#include "xlib/types/type_traits/is_object.hpp"

namespace xlib::execution {

namespace detail {

#if defined(_XLIB_CONCEPTS)

template <typename O>
xlib_concept __can_start = requires(O& o) {
  { ::xlib::execution::start(o) } noexcept;
};

#else  // defined(_XLIB_CONCEPTS)

template <typename O>
xlib_concept __can_start = is_nothrow_invocable_v<decltype(::xlib::execution::start), O>;

#endif  // defined(_XLIB_CONCEPTS)

}  // namespace detail

template <typename O>
xlib_concept operation_state = destructible<O> && is_object_v<O> && detail::__can_start<O>;

}  // namespace xlib::execution

#endif  // XLIB_EXECUTION_CONCEPT_OPERATIONSTATE_HPP_
