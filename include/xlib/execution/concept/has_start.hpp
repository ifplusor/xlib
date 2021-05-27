#ifndef XLIB_EXECUTION_CONCEPT_HASSTART_HPP_
#define XLIB_EXECUTION_CONCEPT_HASSTART_HPP_

#include <utility>  // std::declval

#include "xlib/concepts/concept.hpp"
#include "xlib/types/type_traits/conditional.hpp"
#include "xlib/types/type_traits/integral_constant.hpp"
#include "xlib/types/type_traits/void.hpp"

namespace xlib {
namespace execution {
namespace detail {

#if defined(_XLIB_VARIABLE_TEMPLATES)

// clang-format off
template <typename OperationState>
xlib_requires(__has_member_start_impl,
requires (OperationState& operation_state) (
  operation_state.start()
));
// clang-format on

template <typename OperationState>
xlib_concept __has_member_start = xlib_requires_ref(__has_member_start_impl, OperationState);

// clang-format off
template <typename OperationState>
xlib_requires(__has_nothrow_member_start_impl,
requires (OperationState& operation_state) (
  // FIXME: noexcept
  operation_state.start()
));
// clang-format on

template <typename OperationState>
xlib_concept __has_nothrow_member_start = xlib_requires_ref(__has_nothrow_member_start_impl, OperationState);

#endif  // defined(_XLIB_VARIABLE_TEMPLATES)

template <typename OperationState>
using member_start_result_t = decltype(std::declval<OperationState&>().start());

}  // namespace detail
}  // namespace execution
}  // namespace xlib

#endif  // XLIB_EXECUTION_CONCEPT_HASSTART_HPP_
